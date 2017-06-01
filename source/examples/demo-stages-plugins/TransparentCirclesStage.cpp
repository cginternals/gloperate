
#include "TransparentCirclesStage.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
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


} // namespace


CPPEXPOSE_COMPONENT(TransparentCirclesStage, gloperate::Stage)


TransparentCirclesStage::TransparentCirclesStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, transparencyKernel("transparencyKernel", this, nullptr)
, noiseKernel("noiseKernel", this, nullptr)
{
}

TransparentCirclesStage::~TransparentCirclesStage()
{
}

void TransparentCirclesStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void TransparentCirclesStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();
}

void TransparentCirclesStage::onProcess()
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

void TransparentCirclesStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec2));
    positionBinding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void TransparentCirclesStage::setupProgram()
{
    m_vertexShaderSource   = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoTransparency.vert");
    m_fragmentShaderSource = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoTransparency.frag");

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("transparencyKernel", 0);
    m_program->setUniform("noiseKernel", 1);
}
