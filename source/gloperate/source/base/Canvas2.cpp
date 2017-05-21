
#include <gloperate/base/Canvas2.h>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Renderer.h>


using namespace cppassist;


namespace gloperate
{


Canvas2::Canvas2(Environment * environment)
: cppexpose::Object("canvas")
, m_environment(environment)
, m_openGLContext(nullptr)
, m_renderer(cppassist::make_unique<Renderer>(environment))
{
    info() << "Canvas created";
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
    info() << "Canvas onContextInit";

    m_renderer->onContextInit();
}

void Canvas2::onContextDeinit()
{
    info() << "Canvas onContextDeinit";

    m_renderer->onContextDeinit();
}

void Canvas2::onRender(globjects::Framebuffer * fbo)
{ 
    info() << "Canvas onRender";

    m_renderer->render(fbo);
}

void Canvas2::onUpdate()
{
}

void Canvas2::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;

    info() << "Canvas onViewport";

    m_renderer->onViewport(deviceViewport, virtualViewport);
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
