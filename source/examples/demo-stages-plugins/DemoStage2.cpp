
#include "DemoStage2.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/rendering/Box.h>
#include <gloperate/rendering/Camera.h>


using namespace gloperate;


CPPEXPOSE_COMPONENT(DemoStage2, gloperate::Stage)


DemoStage2::DemoStage2(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "DemoStage2", name)
, renderInterface(this)
, fboOut("fboOut", this, nullptr)
, m_angle(0.0f)
{
}

DemoStage2::~DemoStage2()
{
}

void DemoStage2::onContextInit(gloperate::AbstractGLContext *)
{
    // Create OpenGL objects
    m_camera = cppassist::make_unique<Camera>();
    m_camera->setEye(glm::vec3(0.0, 0.0, 3.0));

    m_texture = std::unique_ptr<globjects::Texture>(m_environment->resourceManager()->load<globjects::Texture>(
        gloperate::dataPath() + "/gloperate/textures/gloperate-logo.glraw"
    ));

    m_box = cppassist::make_unique<Box>();

    m_vertexShader   = std::unique_ptr<globjects::Shader>( m_environment->resourceManager()->load<globjects::Shader>(
        gloperate::dataPath() + "/gloperate/shaders/Geometry/RenderGeometry.vert"
    ));

    m_fragmentShader = std::unique_ptr<globjects::Shader>( m_environment->resourceManager()->load<globjects::Shader>(
        gloperate::dataPath() + "/gloperate/shaders/Geometry/RenderGeometry.frag"
    ));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("tex0", 0);
}

void DemoStage2::onContextDeinit(gloperate::AbstractGLContext *)
{
    // Release OpenGL objects
    m_texture              = nullptr;
    m_box                  = nullptr;
    m_program              = nullptr;
    m_vertexShader         = nullptr;
    m_fragmentShader       = nullptr;
    m_vertexShaderSource   = nullptr;
    m_fragmentShaderSource = nullptr;
}

void DemoStage2::onProcess(gloperate::AbstractGLContext *)
{
    // Animate camera by 90° per second
    m_angle += 0.2f * glm::pi<float>() * (*renderInterface.timeDelta);
    if (m_angle >= 2 * glm::pi<float>()) {
        m_angle -= 2 * glm::pi<float>();
    }

    // Update camera position
    m_camera->setEye(glm::vec3(3.0f * glm::cos(m_angle), 2.0f, 3.0f * glm::sin(m_angle)));

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Get viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;

    // Set viewport
    gl::glViewport((int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

    // Clear screen
    gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    // Set OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);

    // Bind texture
    if (m_texture.get())
    {
        m_texture->bindActive(0);
    }

    // Update uniforms
    m_program->setUniform("viewProjectionMatrix", m_camera->viewProjection());

    // Draw geometry
    m_program->use();
    m_box->draw();
    m_program->release();

    // Unbind texture
    if (m_texture.get())
    {
        m_texture->unbind();
    }

    // Restore OpenGL states
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
    fboOut.setValue(*renderInterface.targetFBO);
}
