
#include <gloperate/stages/demos/DemoTransparencyStage.h>

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{


// Geometry, using squares to display the circles
static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2(-1.f, -1.f),
    glm::vec2(-1.f, +1.f),
    glm::vec2(+1.f, -1.f),
    glm::vec2(+1.f, +1.f),
} };

// Vertex shader displaying the circles
static const char * s_vertexShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 modelViewProjection;
uniform float z;

layout (location = 0) in vec2 a_vertex;

out vec2 v_localPos;

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, z, 1.0);
    v_localPos = a_vertex;
}
)";

// Fragment shader displaying the circles
static const char * s_fragmentShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

const float alpha = 0.35;

uniform sampler2D transparencyKernel;
uniform sampler2D noiseKernel;
uniform vec3 color;
uniform float randVal;

in vec2 v_localPos;

layout (location = 0) out vec4 fragColor;

void main()
{
    // Making a circle from a square
    if (v_localPos.x * v_localPos.x + v_localPos.y * v_localPos.y > 1.0)
    {
        discard;
    }

    float rand = texture(noiseKernel, v_localPos + randVal).r;

    ivec2 transpSize = textureSize(transparencyKernel, 0);
    ivec2 transpIndex = ivec2(rand * transpSize.x, alpha * transpSize.y);
    bool opaque = texelFetch(transparencyKernel, transpIndex, 0).r > 0.0f;

    if (!opaque)
        discard;

    fragColor = vec4(color, 1.0);
}
)";


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoTransparencyStage, gloperate::Stage)


DemoTransparencyStage::DemoTransparencyStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, transparencyKernel("transparencyKernel", this, nullptr)
, noiseKernel("noiseKernel", this, nullptr)
{
}

DemoTransparencyStage::~DemoTransparencyStage()
{
}

void DemoTransparencyStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoTransparencyStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoTransparencyStage::onProcess(AbstractGLContext *)
{
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

    // Bind textures
    if(*transparencyKernel)
        (*transparencyKernel)->bindActive(0);
    if(*noiseKernel)
        (*noiseKernel)->bindActive(1);

    // Prepare random value
    m_program->setUniform("randVal", glm::linearRand(0.0f, 1.0f));

    // Prepare model matrix
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));

    // Draw geometry

    // Red
    m_program->setUniform("modelViewProjection", modelMatrix);
    m_program->setUniform("z", 0.0f);
    m_program->setUniform("color", glm::vec3(1,0,0));

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 2.0f/3.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));

    // Green
    m_program->setUniform("modelViewProjection", modelMatrix);
    m_program->setUniform("z", 0.1f);
    m_program->setUniform("color", glm::vec3(0,1,0));

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, 2.0f/3.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));

    // Blue
    m_program->setUniform("modelViewProjection", modelMatrix);
    m_program->setUniform("z", 0.2f);
    m_program->setUniform("color", glm::vec3(0,0,1));

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    // Unbind textures
    if(*transparencyKernel)
        (*transparencyKernel)->unbindActive(0);
    if(*noiseKernel)
        (*noiseKernel)->unbindActive(1);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoTransparencyStage::setupGeometry()
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

void DemoTransparencyStage::setupProgram()
{
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * fragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = new globjects::Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = new globjects::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = new globjects::Program();
    m_program->attach(m_vertexShader, m_fragmentShader);

    m_program->setUniform("transparencyKernel", 0);
    m_program->setUniform("noiseKernel", 1);
}


} // namespace gloperate
