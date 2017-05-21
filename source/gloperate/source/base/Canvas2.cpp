
#include <gloperate/base/Canvas2.h>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Renderer.h>


namespace gloperate
{


Canvas2::Canvas2(Environment * environment)
: cppexpose::Object("canvas")
, m_environment(environment)
, m_openGLContext(nullptr)
, m_initialized(false)
, m_renderer(cppassist::make_unique<Renderer>(environment))
, m_virtualTime(0.0f)
{
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
        m_renderer->onContextDeinit();

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        m_renderer->onContextInit();
    }

    // Reset status
    m_initialized = false;
}

void Canvas2::updateTime()
{
    // Get number of milliseconds since last call
    auto duration = m_clock.elapsed();
    m_clock.reset();

    // Determine time delta and virtual time
    float timeDelta = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
    m_virtualTime += timeDelta;

    // Update timing
    m_renderer->onUpdateTime(m_virtualTime, timeDelta);

    // [TODO] Check redraw
    redraw();
}

void Canvas2::setViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_renderer->onViewport(deviceViewport, virtualViewport);
    m_initialized = true;
}

void Canvas2::render(globjects::Framebuffer * targetFBO)
{
    if (m_initialized)
    {
        m_renderer->onRender(targetFBO);
    }
}

void Canvas2::promoteKeyPress(int, int)
{
    // [TODO]
}

void Canvas2::promoteKeyRelease(int, int)
{
    // [TODO]
}

void Canvas2::promoteMouseMove(const glm::ivec2 &)
{
    // [TODO]
}

void Canvas2::promoteMousePress(int, const glm::ivec2 &)
{
    // [TODO]
}

void Canvas2::promoteMouseRelease(int, const glm::ivec2 &)
{
    // [TODO]
}

void Canvas2::promoteMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
    // [TODO]
}


} // namespace gloperate
