
#include <gloperate/base/Renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/gl.h>

#include <globjects/globjects.h>
#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/rendering/Box.h>
#include <gloperate/rendering/Camera.h>


using namespace cppassist;


namespace gloperate
{


Renderer::Renderer(Environment * environment)
: cppexpose::Object("renderer")
, m_environment(environment)
, m_angle(0.0f)
{
    info() << "Renderer::Renderer()";
}

Renderer::~Renderer()
{
}

const Environment * Renderer::environment() const
{
    return m_environment;
}

Environment * Renderer::environment()
{
    return m_environment;
}

void Renderer::onContextInit()
{
    info() << "Renderer::onContextInit()";

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

void Renderer::onContextDeinit()
{
    info() << "Renderer::onContextDeinit()";

    // Release OpenGL objects
    m_camera               = nullptr;
    m_texture              = nullptr;
    m_box                  = nullptr;
    m_program              = nullptr;
    m_vertexShader         = nullptr;
    m_fragmentShader       = nullptr;
    m_vertexShaderSource   = nullptr;
    m_fragmentShaderSource = nullptr;
}

void Renderer::onUpdateTime(float, float timeDelta)
{
    info() << "Renderer::onUpdateTime()";

    // Animate camera by 90° per second
    m_angle += 0.5f * glm::pi<float>() * timeDelta;
    if (m_angle >= 2 * glm::pi<float>()) {
        m_angle -= 2 * glm::pi<float>();
    }

    // Update camera position
    m_camera->setEye(glm::vec3(3.0f * glm::cos(m_angle), 2.0f, 3.0f * glm::sin(m_angle)));
}

void Renderer::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    info() << "Renderer::onViewport()";

    // Save viewport size
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;
}

void Renderer::onRender(globjects::Framebuffer * fbo)
{ 
    info() << "Renderer::onRender()";

    // Bind FBO
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    gl::glViewport((int)m_deviceViewport.x, (int)m_deviceViewport.y, (int)m_deviceViewport.z, (int)m_deviceViewport.w);

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
}


} // namespace gloperate
