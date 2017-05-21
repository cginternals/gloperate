
#include <gloperate/base/Canvas2.h>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Environment.h>

// demo
#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>
#include <globjects/base/AbstractStringSource.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <gloperate/gloperate.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>
#include <iostream>


namespace gloperate
{


Canvas2::Canvas2(Environment * environment)
: cppexpose::Object("canvas")
, m_environment(environment)
, m_openGLContext(nullptr)
{
    std::cout << "Canvas created" << std::endl;
}

Canvas2::~Canvas2()
{
}

const Environment * Canvas2::environment() const
{
    return m_environment;
}

Environment * Canvas2::environment()
{
    return m_environment;
}

const AbstractGLContext * Canvas2::openGLContext() const
{
    return m_openGLContext;
}

AbstractGLContext * Canvas2::openGLContext()
{
    return m_openGLContext;
}

void Canvas2::setOpenGLContext(AbstractGLContext * context)
{
    // Deinitialize renderer in old context
    if (m_openGLContext)
    {
        onContextDeinit();

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        onContextInit();
    }
}

void Canvas2::render(globjects::Framebuffer * targetFBO)
{
    onRender(targetFBO);
}

void Canvas2::onContextInit()
{
    std::cout << "Canvas onContextInit" << std::endl;

    // demo
    m_texture = std::unique_ptr<globjects::Texture>(m_environment->resourceManager()->load<globjects::Texture>(
        gloperate::dataPath() + "/gloperate/textures/gloperate-logo.glraw"
    ));

    m_quad = cppassist::make_unique<ScreenAlignedQuad>();

    m_vertexShaderSource   = m_quad->vertexShaderSource();
    m_fragmentShaderSource = m_quad->fragmentShaderSource();
    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("source", 0);
}

void Canvas2::onContextDeinit()
{
    std::cout << "Canvas onContextDeinit" << std::endl;

    // demo
    m_texture = nullptr;
    m_quad = nullptr;
    m_program = nullptr;
    m_vertexShader = nullptr;
    m_fragmentShader = nullptr;
    m_vertexShaderSource = nullptr;
    m_fragmentShaderSource = nullptr;
}

void Canvas2::onRender(globjects::Framebuffer * fbo)
{ 
    std::cout << "Canvas onRender" << std::endl;

    globjects::setCurrentContext();

    // Bind FBO
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    int ofs = 50;
    gl::glViewport((int)m_deviceViewport.x + ofs, (int)m_deviceViewport.y + ofs, (int)m_deviceViewport.z - 2*ofs, (int)m_deviceViewport.w - 2*ofs);

    // Set OpenGL states
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Bind texture
    if (m_texture.get())
    {
        m_texture->bindActive(0);
    }

    // Draw screen-aligned quad
    m_program->use();
    m_quad->draw();
    m_program->release();

    // Unbind texture
    if (m_texture.get())
    {
        m_texture->unbind();
    }

    // Restore OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);
}

void Canvas2::onUpdate()
{
}

void Canvas2::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;
    std::cout << "Canvas onViewport" << std::endl;
}

void Canvas2::onKeyPress(int, int)
{
}

void Canvas2::onKeyRelease(int, int)
{
}

void Canvas2::onMouseMove(const glm::ivec2 &)
{
}

void Canvas2::onMousePress(int, const glm::ivec2 &)
{
}

void Canvas2::onMouseRelease(int, const glm::ivec2 &)
{
}

void Canvas2::onMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
}


} // namespace gloperate
