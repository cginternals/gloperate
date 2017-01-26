
#include <gloperate/stages/demos/DemoSSAOPostprocessingStage.h>

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/NamedString.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{


// Geometry, screen aligned quad
static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2(-1.f, -1.f),
    glm::vec2(-1.f, +1.f),
    glm::vec2(+1.f, -1.f),
    glm::vec2(+1.f, +1.f),
} };

// Vertex shader displaying the quad
static const char * s_vertexShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 a_vertex;

out vec2 v_uv;

void main()
{
    gl_Position = vec4(a_vertex, 0.0, 1.0);
    v_uv = a_vertex * 0.5 + 0.5;
}
)";

// Fragment shader displaying the quad
static const char * s_fragmentShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shading_language_include : require

#include </ssao.glsl>

const vec3 ssaoColor = vec3(0.0);
const float farZ = 10.0;
const float ssaoRadius = 0.2;
const float ssaoIntensity = 0.2;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler1D ssaoKernelTexture;
uniform sampler2D ssaoNoiseTexture;

uniform mat4 projectionMatrix;
uniform mat4 projectionInverseMatrix;
uniform mat3 normalMatrix;

in vec2 v_uv;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 baseColor = texture(colorTexture, v_uv).rgb;

    vec3 ssaoFactor = ssao(
        v_uv,
        ssaoColor,
        farZ,
        ssaoRadius,
        ssaoIntensity,
        ssaoKernelTexture,
        ssaoNoiseTexture,
        depthTexture,
        normalTexture,
        projectionMatrix,
        projectionInverseMatrix,
        normalMatrix
    );

    fragColor = vec4(baseColor * ssaoFactor, 1.0);
}
)";


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoSSAOPostprocessingStage, gloperate::Stage)


DemoSSAOPostprocessingStage::DemoSSAOPostprocessingStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, colorTexture("colorTexture", this, nullptr)
, normalTexture("normalTexture", this, nullptr)
, depthTexture("depthTexture", this, nullptr)
, ssaoKernel("ssaoKernel", this, nullptr)
, ssaoNoise("ssaoNoise", this, nullptr)
, projectionMatrix("projectionMatrix", this)
, normalMatrix("normalMatrix", this)
, sceneRendered("sceneRendered", this, false)
{
}

DemoSSAOPostprocessingStage::~DemoSSAOPostprocessingStage()
{
}

void DemoSSAOPostprocessingStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoSSAOPostprocessingStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoSSAOPostprocessingStage::onProcess(AbstractGLContext *)
{
    if (!(*colorTexture && *normalTexture && *depthTexture && *ssaoKernel && *ssaoNoise))
    {
        renderInterface.rendered.setValue(false);
        return;
    }

    // Get viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    if (!fbo) fbo = globjects::Framebuffer::defaultFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Set uniforms
    m_program->setUniform("projectionMatrix", *projectionMatrix);
    m_program->setUniform("projectionInverseMatrix", glm::inverse(*projectionMatrix));
    m_program->setUniform("normalMatrix", *normalMatrix);

    // Bind textures
    (*colorTexture) ->bindActive(0);
    (*normalTexture)->bindActive(1);
    (*depthTexture) ->bindActive(2);
    (*ssaoKernel)   ->bindActive(3);
    (*ssaoNoise)    ->bindActive(4);

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    // Unbind textures
    (*colorTexture) ->unbindActive(0);
    (*normalTexture)->unbindActive(1);
    (*depthTexture) ->unbindActive(2);
    (*ssaoKernel)   ->unbindActive(3);
    (*ssaoNoise)    ->unbindActive(4);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoSSAOPostprocessingStage::setupGeometry()
{
    m_vao = new globjects::VertexArray;
    m_vertexBuffer = new globjects::Buffer();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec2));
    positionBinding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void DemoSSAOPostprocessingStage::setupProgram()
{
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * fragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    globjects::NamedString::create("/ssao.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/ssao.glsl"));

    m_vertexShader   = new globjects::Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = new globjects::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = new globjects::Program();
    m_program->attach(m_vertexShader, m_fragmentShader);

    m_program->setUniform("colorTexture", 0);
    m_program->setUniform("normalTexture", 1);
    m_program->setUniform("depthTexture", 2);
    m_program->setUniform("ssaoKernelTexture", 3);
    m_program->setUniform("ssaoNoiseTexture", 4);

}


} // namespace gloperate
