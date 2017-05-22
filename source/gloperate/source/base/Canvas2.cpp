
#include <gloperate/base/Canvas2.h>

#include <glm/glm.hpp>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Slot.h>


namespace
{


template <typename T>
gloperate::Slot<T> * getSlot(gloperate::Stage * stage, const std::string & name)
{
    if (!stage) {
        return nullptr;
    } else {
        return static_cast<gloperate::Slot<T> *>(stage->property(name));
    }
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

const Stage * Canvas2::renderStage() const
{
    return m_renderStage.get();
}

Stage * Canvas2::renderStage()
{
    return m_renderStage.get();
}

void Canvas2::setRenderStage(std::unique_ptr<Stage> && stage)
{
    m_newStage = std::move(stage);
    redraw();
}

void Canvas2::loadRenderStage(const std::string & name)
{
    // Find component
    auto component = m_environment->componentManager()->component<gloperate::Stage>(name);
    if (component)
    {
        // Create stage
        m_newStage = std::unique_ptr<Stage>( component->createInstance(m_environment) );
    }
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
        cppassist::debug(2, "gloperate") << "deinitContext()";

        if (m_renderStage)
        {
            m_renderStage->deinitContext(m_openGLContext);
        }

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        cppassist::debug(2, "gloperate") << "initContext()";

        m_openGLContext = context;

        if (m_renderStage)
        {
            m_renderStage->initContext(m_openGLContext);
        }
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
    // Store viewport information
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;
    m_initialized = true;

    // Promote new viewport
    auto slotdeviceViewport = getSlot<glm::vec4>(m_renderStage.get(), "deviceViewport");
    if (slotdeviceViewport) slotdeviceViewport->setValue(m_deviceViewport);
    auto slotVirtualViewport = getSlot<glm::vec4>(m_renderStage.get(), "virtualViewport");
    if (slotVirtualViewport) slotVirtualViewport->setValue(m_virtualViewport);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::render(globjects::Framebuffer * targetFBO)
{
    auto fboName = targetFBO->hasName() ? targetFBO->name() : std::to_string(targetFBO->id());
    cppassist::debug(2, "gloperate") << "render(); " << "targetFBO: " << fboName;

    // Abort if not initialized
    if (!m_initialized) return;

    // Check if the render stage is to be replaced
    if (m_newStage)
    {
        // Destroy old stage
        if (m_renderStage)
        {
            // Deinitialize stage
            m_renderStage->deinitContext(m_openGLContext);
        }

        // Move stage
        m_renderStage.reset(m_newStage.release());
        m_newStage.reset();

        // Initialize stage
        m_renderStage->initContext(m_openGLContext);

        // Promote viewport information
        auto slotdeviceViewport = getSlot<glm::vec4>(m_renderStage.get(), "deviceViewport");
        if (slotdeviceViewport) slotdeviceViewport->setValue(m_deviceViewport);
        auto slotVirtualViewport = getSlot<glm::vec4>(m_renderStage.get(), "virtualViewport");
        if (slotVirtualViewport) slotVirtualViewport->setValue(m_virtualViewport);

        // Mark output as required
        auto slotRendered = getSlot<bool>(m_renderStage.get(), "rendered");
        if (slotRendered) slotRendered->setRequired(true);
    }

    // Render
    auto slotTargetFBO = getSlot<globjects::Framebuffer *>(m_renderStage.get(), "targetFBO");
    if (slotTargetFBO)
    {
        slotTargetFBO->setValue(targetFBO);
        m_renderStage->process(m_openGLContext);
    }
}

void Canvas2::promoteKeyPress(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "keyPressed(" << key << ", " << modifier << ")";

    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteKeyRelease(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "keyReleased(" << key << ", " << modifier << ")";

    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseMove(const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseMoved(" << pos.x << ", " << pos.y << ")";

    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    // [TODO]

    // Check if a redraw is required
    checkRedraw();
}

void Canvas2::promoteMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

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
