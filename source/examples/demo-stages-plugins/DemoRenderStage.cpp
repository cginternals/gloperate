
#include "DemoRenderStage.h"

#include <random>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


// Geometry describing the triangle
static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2( +1.f, -1.f ),
    glm::vec2( +1.f, +1.f ),
    glm::vec2( -1.f, -1.f ),
    glm::vec2( -1.f, +1.f ) } };


CPPEXPOSE_COMPONENT(DemoRenderStage, gloperate::Stage)


DemoRenderStage::DemoRenderStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, texture        ("texture",         this, nullptr)
, angle          ("angle",           this, 0.0f)
, colorTexture   ("colorTexture",    this, nullptr)
, program        ("program",         this, nullptr)
, fboOut         ("fboOut",          this, nullptr)
, colorTextureOut("colorTextureOut", this, nullptr)
{
}

DemoRenderStage::~DemoRenderStage()
{
}

void DemoRenderStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupCamera();
}

void DemoRenderStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void DemoRenderStage::onProcess()
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

    // Get model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::rotate(modelMatrix, *angle, glm::vec3(0.0, 1.0, 0.0));

    // Update model-view-projection matrix
    (*program)->setUniform("source", 0);
    (*program)->setUniform("viewProjectionMatrix",      m_camera.viewProjectionMatrix());
    (*program)->setUniform("modelViewProjectionMatrix", m_camera.viewProjectionMatrix() * modelMatrix);

    // Bind texture
    if (*texture) {
        (*texture)->bindActive(0);
    }

    // Draw geometry
    (*program)->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    (*program)->release();

    // Unbind texture
    if (*texture) {
        (*texture)->unbindActive(0);
    }

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Indicate change to the output FBO and texture
    fboOut.setValue(fbo);
    colorTextureOut.setValue(*colorTexture);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoRenderStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void DemoRenderStage::setupCamera()
{
    m_camera.setEye(glm::vec3(0.0, 0.0, 12.0));
}
