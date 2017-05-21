
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
{
    info() << "Renderer created";
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

void Renderer::render(globjects::Framebuffer * targetFBO)
{
    onRender(targetFBO);
}

void Renderer::onContextInit()
{
    info() << "Renderer onContextInit";

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
    info() << "Renderer onContextDeinit";

    m_camera = nullptr;
    m_texture = nullptr;
    m_box = nullptr;
    m_program = nullptr;
    m_vertexShader = nullptr;
    m_fragmentShader = nullptr;
    m_vertexShaderSource = nullptr;
    m_fragmentShaderSource = nullptr;
}

void Renderer::onRender(globjects::Framebuffer * fbo)
{ 
    info() << "Renderer onRender";

    globjects::setCurrentContext();

    // Bind FBO
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    gl::glViewport((int)m_deviceViewport.x, (int)m_deviceViewport.y, (int)m_deviceViewport.z, (int)m_deviceViewport.w);

    // Set OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);

    // Bind texture
    if (m_texture.get())
    {
        m_texture->bindActive(0);
    }

    // Animate camera
    static float angle = 0.0f;

    angle += 0.03f;
    if (angle >= 2 * glm::pi<float>()) angle -= 2 * glm::pi<float>();

    float x = 3.0f * glm::sin(angle);
    float y = 3.0f * glm::cos(angle);
    m_camera->setEye(glm::vec3(y, 2.0f, x));

    // Update uniforms
    m_program->setUniform("viewProjectionMatrix", m_camera->viewProjection());

    // Draw screen-aligned quad
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

void Renderer::onUpdate()
{
}

void Renderer::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;
    info() << "Renderer onViewport";
}


} // namespace gloperate
