
#include <gloperate/base/Canvas2.h>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>
#include <gloperate/base/Renderer.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Slot.h>


namespace
{


template <typename T>
gloperate::Slot<T> * getSlot(gloperate::Stage * stage, const std::string & name)
{
    return static_cast<gloperate::Slot<T> *>(stage->property(name));
}


}


namespace gloperate
{


Canvas2::Canvas2(Environment * environment)
: cppexpose::Object("canvas")
, m_environment(environment)
, m_openGLContext(nullptr)
, m_initialized(false)
, m_virtualTime(0.0f)
{
    auto component = m_environment->componentManager()->component<gloperate::Stage>("DemoStage");
    if (component) {
        m_renderStage = std::unique_ptr<Stage>( component->createInstance(environment) );
    }
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
        m_renderStage->deinitContext(m_openGLContext);

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        m_renderStage->initContext(m_openGLContext);
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
    auto slotTimeDelta = getSlot<float>(m_renderStage.get(), "timeDelta");
    if (slotTimeDelta) slotTimeDelta->setValue(timeDelta);
    auto slotVirtualTime = getSlot<float>(m_renderStage.get(), "virtualTime");
    if (slotVirtualTime) slotVirtualTime->setValue(m_virtualTime);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::setViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    // Promote new viewport
    auto slotdeviceViewport = getSlot<glm::vec4>(m_renderStage.get(), "deviceViewport");
    if (slotdeviceViewport) slotdeviceViewport->setValue(deviceViewport);
    auto slotVirtualViewport = getSlot<glm::vec4>(m_renderStage.get(), "virtualViewport");
    if (slotVirtualViewport) slotVirtualViewport->setValue(virtualViewport);
    m_initialized = true;

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::render(globjects::Framebuffer * targetFBO)
{
    // Abort if not initialized
    if (!m_initialized) return;

    // Render
    auto slotTargetFBO = getSlot<globjects::Framebuffer *>(m_renderStage.get(), "targetFBO");
    if (slotTargetFBO)
    {
        slotTargetFBO->setValue(targetFBO);
        m_renderStage->process(m_openGLContext);
    }
}

void Canvas2::promoteKeyPress(int, int)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteKeyRelease(int, int)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseMove(const glm::ivec2 &)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMousePress(int, const glm::ivec2 &)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseRelease(int, const glm::ivec2 &)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::checkRedraw()
{
    auto slotRendered = getSlot<globjects::Framebuffer *>(m_renderStage.get(), "rendered");
    if (slotRendered && !slotRendered->isValid())
    {
        redraw();
    }
}


} // namespace gloperate
