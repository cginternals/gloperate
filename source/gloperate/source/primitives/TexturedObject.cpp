
#include <gloperate/primitives/TexturedObject.h>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/base/StaticStringSource.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/globjects.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>

#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/tools/DistanceMap.h>
#include <gloperate/tools/MasksTableGenerator.h>


using namespace gl;
using namespace glm;
using namespace globjects;

namespace gloperate
{


const char * TexturedObject::s_defaultVertexShaderSource = R"(
#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in vec3 a_tangent;
layout(location = 4) in vec3 a_binormal;

out vec3 v_tanEyeVec;
out vec3 v_tanLightVec;
out vec3 v_texCoord;
flat out float v_rand;

uniform mat4 transform;
uniform vec3 eyeCoord;
uniform vec3 lightCoord;
uniform float invBumpDepth;
uniform bool useNormals;
uniform bool useHeights;

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
	v_texCoord = vec3(a_texCoord, 1.0);
    v_rand = gl_VertexID;

	if( useHeights )
	{
    vec3 eyeVec = eyeCoord - a_vertex;
    v_tanEyeVec.x = dot(a_tangent, eyeVec);
    v_tanEyeVec.y = dot(a_binormal, eyeVec);
    v_tanEyeVec.z = - invBumpDepth * dot(a_normal, eyeVec);
	}
	
	if( useNormals )
	{
    vec3 lightVec = lightCoord - a_vertex;
    v_tanLightVec.x = dot(a_tangent, lightVec);
    v_tanLightVec.y = dot(a_binormal, lightVec);
    v_tanLightVec.z = dot(a_normal, lightVec);
  }
}
)";

const char* TexturedObject::s_defaultFagmentShaderSource = R"(
#version 150 core
#extension GL_ARB_sample_shading : require
#extension GL_ARB_explicit_attrib_location : require

in vec3 v_tanEyeVec;
in vec3 v_tanLightVec;
in vec3 v_texCoord;
flat in float v_rand;

out vec4 fragColor;

uniform sampler2D colorTex;
uniform sampler2D normalTex;
uniform sampler3D distanceTex;
uniform bool useColors;
uniform bool useNormals;
uniform bool useHeights;
uniform vec3 normalizationFactor;
uniform int numIterations;

uniform sampler2D maskTex;
uniform vec2 viewport;

float rand();
float calculateAlpha(uint mask);

const float denormFactor = pow(2.0, 8.0) - 1.0;

void main()
{
  vec2 dx = dFdx(v_texCoord.xy);
  vec2 dy = dFdy(v_texCoord.xy);

  vec3 texCoord = v_texCoord;
  if( useHeights )
  {
    vec3 offset = normalize(v_tanEyeVec);
    offset *= normalizationFactor;
    offset.z = - abs(offset.z); // Make the displacement mapping work for backfaces, too.

    // ray marching
    for(int i = 0; i < numIterations; i++)
    {
      float distance = texture(distanceTex, texCoord).r;
      texCoord += distance * offset;
    }
  }

  float diffuse = 1;
  if( useNormals )
  {
    vec3 tanNormal = 2 * textureGrad(normalTex, texCoord.xy, dx, dy).rgb - 1;
    vec3 tanLightVec = normalize(v_tanLightVec);
    diffuse = dot(tanNormal, tanLightVec);
  }
  
	vec4 baseColor = vec4(0.5, 0.5, 0.5, 1.0);
	if( useColors )
	{
    baseColor = textureGrad(colorTex, texCoord.xy, dx, dy);
	}
	
	fragColor = vec4(diffuse * baseColor.rgb, baseColor.a);

	// transparency
    ivec2 index = ivec2(rand() * 1023.0, fragColor.a * 255);
    uint mask = uint(texelFetch(maskTex, index, 0).r * denormFactor);

    uint sampleBit = 1u << gl_SampleID;
    if ((mask & sampleBit) != sampleBit)
        discard;

    fragColor = vec4(fragColor.rgb, 1.0);
}

highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

float rand()
{
    vec2 normFragCoord = floor(gl_FragCoord.xy) / viewport * v_rand;
    return rand(normFragCoord.xy);
}
)";


TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, PolygonalDrawable * drawable, globjects::Shader * vertexShader, globjects::Shader * fragmentShader, globjects::Program * program)
: m_polygonalDrawable(drawable)
, m_vao(vao)
, m_vaoElementCount(vaoElementCount)
, m_program(program)
, m_vertexShader(vertexShader)
, m_fragmentShader(fragmentShader)
, m_colorTexture(nullptr)
, m_normalTexture(nullptr)
, m_heightTexture(nullptr)
, m_distanceTexture(nullptr)
, m_colorSamplerIndex(0)
, m_normalSamplerIndex(1)
, m_distanceSamplerIndex(2)
, m_maskSamplerIndex(3)
, m_textureWidth(1)
, m_textureHeight(1)
, m_iterations(16)
, m_lightPosition(glm::vec3(2, 2, 2))
, m_useColors(false)
, m_useNormals(false)
, m_useHeights(false)
, m_inverseBumpDepth(0.5f)
, m_depthLayers(16)
, m_targetFramebufferCapability(targetFramebufferCapability)
, m_viewportCapability(viewportCapability)
{
	StringTemplate* vertexShaderSource = new StringTemplate(new StaticStringSource(s_defaultVertexShaderSource));
	StringTemplate* fragmentShaderSource = new StringTemplate(new StaticStringSource(s_defaultFagmentShaderSource));

#ifdef __APPLE__
	vertexShaderSource->replace("#version 140", "#version 150");
	fragmentShaderSource->replace("#version 140", "#version 150");
#endif

	if (m_program == nullptr)
	{
		m_program = new Program();

		if (!m_vertexShader) {
			m_vertexShader = new Shader(gl::GL_VERTEX_SHADER, vertexShaderSource);
		}

		if (!fragmentShader) {
			m_fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
		}

		m_program->attach(m_vertexShader, m_fragmentShader);
	}

	initialize();
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Shader * vertexShader, globjects::Shader * fragmentShader)
: TexturedObject(targetFramebufferCapability, viewportCapability, nullptr, 0, drawable, vertexShader, fragmentShader, nullptr)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Program * program)
: TexturedObject(targetFramebufferCapability, viewportCapability, nullptr, 0, drawable, nullptr, nullptr, program)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable, globjects::Shader * fragmentShader)
: TexturedObject(targetFramebufferCapability, viewportCapability, nullptr, 0, drawable, nullptr, fragmentShader, nullptr)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, PolygonalDrawable * drawable)
: TexturedObject(targetFramebufferCapability, viewportCapability, nullptr, 0, drawable, nullptr, nullptr, nullptr)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Shader * vertexShader, globjects::Shader * fragmentShader)
: TexturedObject(targetFramebufferCapability, viewportCapability, vao, vaoElementCount, nullptr, vertexShader, fragmentShader, nullptr)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Program * program)
: TexturedObject(targetFramebufferCapability, viewportCapability, vao, vaoElementCount, nullptr, nullptr, nullptr, program)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount, globjects::Shader * fragmentShader)
: TexturedObject(targetFramebufferCapability, viewportCapability, vao, vaoElementCount, nullptr, nullptr, fragmentShader, nullptr)
{
}

TexturedObject::TexturedObject(gloperate::AbstractTargetFramebufferCapability * targetFramebufferCapability, gloperate::AbstractViewportCapability * viewportCapability, globjects::VertexArray * vao, int vaoElementCount)
: TexturedObject(targetFramebufferCapability, viewportCapability, vao, vaoElementCount, nullptr, nullptr, nullptr, nullptr)
{
}

void TexturedObject::initialize()
{
	setupFramebuffer();
	setupMaskTexture();

	setColorSamplerUniform(0);
	setNormalSamplerUniform(1);
	setDistanceSamplerUniform(2);
	setMaskSamplerUniform(3);
}

void TexturedObject::draw()
{
	if (m_viewportCapability->hasChanged())
	{
		glViewport(
			m_viewportCapability->x(),
			m_viewportCapability->y(),
			m_viewportCapability->width(),
			m_viewportCapability->height());

		m_viewportCapability->setChanged(false);

		const auto viewport = glm::vec2{ m_viewportCapability->width(), m_viewportCapability->height() };
		m_program->setUniform("viewport", viewport);

		updateFramebuffer();
	}

	clearBuffers();

	renderOpaqueGeometry();

	glEnable(GL_SAMPLE_SHADING);
	glMinSampleShading(1.0);

	renderAlphaToCoverage(gl::GL_COLOR_ATTACHMENT0);

	glDisable(GL_SAMPLE_SHADING);

	blit();

	Framebuffer::unbind(GL_FRAMEBUFFER);
}

void TexturedObject::setupFramebuffer()
{
	m_opaqueColorAttachment = make_ref<Texture>(GL_TEXTURE_2D_MULTISAMPLE);
	m_depthAttachment = make_ref<Texture>(GL_TEXTURE_2D_MULTISAMPLE);

	updateFramebuffer();

	m_fbo = make_ref<Framebuffer>();

	m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_opaqueColorAttachment);
	m_fbo->attachTexture(GL_DEPTH_ATTACHMENT, m_depthAttachment);

	m_fbo->printStatus(true);
}

void TexturedObject::setupMaskTexture()
{
	const auto table = MasksTableGenerator::generateDistributions(s_numSamples);

	m_maskTexture = Texture::createDefault(GL_TEXTURE_2D);
	m_maskTexture->image2D(0, GL_R8, table->at(0).size(), table->size(), 0, GL_RED, GL_UNSIGNED_BYTE, table->data());
}

void TexturedObject::updateFramebuffer()
{
	const auto size = glm::ivec2{ m_viewportCapability->width(), m_viewportCapability->height() };

	m_opaqueColorAttachment->image2DMultisample(s_numSamples, GL_RGBA8, size, GL_FALSE);
	m_depthAttachment->image2DMultisample(s_numSamples, GL_DEPTH_COMPONENT, size, GL_FALSE);
}

void TexturedObject::clearBuffers()
{
	m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });

	m_fbo->clearBuffer(GL_COLOR, 0, glm::vec4(0.85f, 0.87f, 0.91f, 1.0f));
	m_fbo->clearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0.0f);
}

void TexturedObject::renderOpaqueGeometry()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	m_fbo->bind(GL_FRAMEBUFFER);
	m_fbo->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);
}

void TexturedObject::renderAlphaToCoverage(gl::GLenum colorAttachment)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	m_fbo->bind(GL_FRAMEBUFFER);
	m_fbo->setDrawBuffer(colorAttachment);

	if (m_colorTexture) {
		gl::glActiveTexture(gl::GL_TEXTURE0 + m_colorSamplerIndex);
		m_colorTexture->bind();
	}
	if (m_normalTexture) {
		gl::glActiveTexture(gl::GL_TEXTURE0 + m_normalSamplerIndex);
		m_normalTexture->bind();
	}
	if (m_distanceTexture) {
		gl::glActiveTexture(gl::GL_TEXTURE0 + m_distanceSamplerIndex);
		m_distanceTexture->bind();
	}
	gl::glActiveTexture(gl::GL_TEXTURE0 + m_maskSamplerIndex);
	m_maskTexture->bind();

	m_program->setUniform(m_program->getUniformLocation("transform"), m_transform);
	m_program->setUniform(m_program->getUniformLocation("eyeCoord"), m_eye);
	m_program->setUniform(m_program->getUniformLocation("lightCoord"), m_lightPosition);
	m_program->setUniform(m_program->getUniformLocation("colorTex"), m_colorSamplerIndex);
	m_program->setUniform(m_program->getUniformLocation("normalTex"), m_normalSamplerIndex);
	m_program->setUniform(m_program->getUniformLocation("distanceTex"), m_distanceSamplerIndex);
	m_program->setUniform(m_program->getUniformLocation("maskTex"), m_maskSamplerIndex);
	m_program->setUniform(m_program->getUniformLocation("useColors"), m_useColors);
	m_program->setUniform(m_program->getUniformLocation("useNormals"), m_useNormals);
	m_program->setUniform(m_program->getUniformLocation("useHeights"), m_useHeights);
	m_program->setUniform(m_program->getUniformLocation("normalizationFactor"), glm::vec3(-m_depthLayers / (float)m_textureWidth, -m_depthLayers / (float)m_textureHeight, 1.0));
	m_program->setUniform(m_program->getUniformLocation("numIterations"), m_iterations);
	m_program->setUniform(m_program->getUniformLocation("invBumpDepth"), m_inverseBumpDepth);

	m_program->use();
	if (m_vao)
	{
		m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, m_vaoElementCount);
	}
	else
	{
		m_polygonalDrawable->draw();
	}
	m_program->release();

	if (m_colorTexture) {
		m_colorTexture->unbind();
	}
	if (m_normalTexture) {
		m_normalTexture->unbind();
	}
	if (m_distanceTexture) {
		m_distanceTexture->unbind();
	}

	glDisable(GL_DEPTH_TEST);
}

void TexturedObject::blit()
{
	auto targetfbo = m_targetFramebufferCapability->framebuffer();
	auto drawBuffer = GL_COLOR_ATTACHMENT0;

	if (!targetfbo)
	{
		targetfbo = Framebuffer::defaultFBO();
		drawBuffer = GL_BACK_LEFT;
	}

	const auto rect = std::array<GLint, 4>{{
			m_viewportCapability->x(),
			m_viewportCapability->y(),
			m_viewportCapability->width(),
			m_viewportCapability->height()
		}};

	m_fbo->bind();
	
	m_fbo->blit(gl::GL_COLOR_ATTACHMENT0, rect, targetfbo, drawBuffer, rect,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void TexturedObject::setTransform(mat4 transform)
{
	m_transform = transform;
}

void TexturedObject::setEye(vec3 eye)
{
	m_eye = eye;
}

void TexturedObject::setColorTexture(Texture* texture)
{
	m_colorTexture = texture;
	m_useColors = true;
}

const Texture * TexturedObject::colorTexture() const
{
	return m_colorTexture;
}

Texture * TexturedObject::colorTexture()
{
	return m_colorTexture;
}

void TexturedObject::setNormalTexture(Texture* texture)
{
	m_normalTexture = texture;
	m_useNormals = true;
}

const Texture * TexturedObject::normalTexture() const
{
	return m_normalTexture;
}

Texture * TexturedObject::normalTexture()
{
	return m_normalTexture;
}

void TexturedObject::setHeightTexture(Texture* texture)
{
	m_heightTexture = texture;
	m_useHeights = true;

	updateDistanceTexture();
}

void TexturedObject::updateDistanceTexture()
{
	if (!m_heightTexture) {
		return;
	}

	m_textureWidth = m_heightTexture->getLevelParameter(0, GL_TEXTURE_WIDTH);
	m_textureHeight = m_heightTexture->getLevelParameter(0, GL_TEXTURE_HEIGHT);
	GLint alignment = globjects::getInteger(GL_PACK_ALIGNMENT);
	GLint alignedWidth = m_textureWidth;
	if ((alignedWidth * 4) % alignment != 0) { // 4 is the size of a GL_FLOAT in bytes
		/*
		The only possible case where the size of an image row that consists of GL_FLOATs
		(i.e. the row size is a multiple of 4) does not line up with the alignment is when
		GL_PACK_ALIGNMENT == 8 because GL_PACK_ALIGNMENT can only be 1, 2, 4 or 8.
		Therefore we can simply increment alignedWidth here.
		*/
		alignedWidth++;
	}
	float * heightMap = new float[alignedWidth * m_textureHeight];
	float * distanceMap = new float[alignedWidth * m_textureHeight * m_depthLayers];
	m_heightTexture->getImage(0, GL_RED, GL_FLOAT, heightMap);

	DistanceMap::init_distance_map(heightMap, distanceMap, m_textureWidth, alignedWidth, m_textureHeight, m_depthLayers);

	m_distanceTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_3D);
	m_distanceTexture->image3D(0, gl::GL_RED, glm::vec3(m_textureWidth, m_textureHeight, m_depthLayers), 0, gl::GL_RED, gl::GL_FLOAT, distanceMap);

	m_distanceTexture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
	m_distanceTexture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
	m_distanceTexture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	m_distanceTexture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	m_distanceTexture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);

	delete[] distanceMap;
	delete[] heightMap;
}

const Texture * TexturedObject::heightTexture() const
{
	return m_heightTexture;
}

Texture * TexturedObject::heightTexture()
{
	return m_heightTexture;
}

void TexturedObject::setColorSamplerUniform(int index)
{
	m_colorSamplerIndex = index;
	m_program->setUniform("colorTex", m_colorSamplerIndex);
}

void TexturedObject::setNormalSamplerUniform(int index)
{
	m_normalSamplerIndex = index;
	m_program->setUniform("normalTex", m_normalSamplerIndex);
}

void TexturedObject::setDistanceSamplerUniform(int index)
{
	m_distanceSamplerIndex = index;
	m_program->setUniform("distanceTex", m_distanceSamplerIndex);
}

void TexturedObject::setMaskSamplerUniform(int index)
{
	m_maskSamplerIndex = index;
	m_program->setUniform("maskTex", m_maskSamplerIndex);
}

PolygonalDrawable * TexturedObject::polygonalDrawable() const
{
	return m_polygonalDrawable;
}

globjects::VertexArray * TexturedObject::vertexArray() const
{
	return m_vao;
}

Program * TexturedObject::program() const
{
	return m_program;
}

Shader * TexturedObject::vertexShader() const
{
	return m_vertexShader;
}

Shader * TexturedObject::fragmentShader() const
{
	return m_fragmentShader;
}

void TexturedObject::setIterations(unsigned short iterations)
{
	m_iterations = iterations;
}

unsigned short TexturedObject::iterations() const
{
	return m_iterations;
}

void TexturedObject::setLightPosition(glm::vec3 position)
{
	m_lightPosition = position;
}

glm::vec3 TexturedObject::lightPosition() const
{
	return m_lightPosition;
}

void TexturedObject::setUseColors(bool useColors)
{
	m_useColors = useColors;
}

bool TexturedObject::useColors() const
{
	return m_useColors;
}

void TexturedObject::setUseNormals(bool useNormals)
{
	m_useNormals = useNormals;
}

bool TexturedObject::useNormals() const
{
	return m_useNormals;
}

void TexturedObject::setUseHeights(bool useHeights)
{
	m_useHeights = useHeights;
}

bool TexturedObject::useHeights() const
{
	return m_useHeights;
}

void TexturedObject::setInverseBumpDepth(float invBumpDepth)
{
	m_inverseBumpDepth = invBumpDepth;
}

float TexturedObject::inverseBumpDepth() const
{
	return m_inverseBumpDepth;
}

void TexturedObject::setDepthLayers(unsigned short depthLayers)
{
	unsigned short previousValue = m_depthLayers;
	m_depthLayers = depthLayers;

	if (m_depthLayers != previousValue)
	{
		updateDistanceTexture();
	}
}

unsigned short TexturedObject::depthLayers() const
{
	return m_depthLayers;
}

} // gloperate
