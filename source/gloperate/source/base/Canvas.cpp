
#include <gloperate/base/Canvas.h>

#include <glm/glm.hpp>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>
#include <cppassist/string/manipulation.h>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Slot.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>


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

auto s_nextCanvasId = size_t(0);


}


namespace gloperate
{


Canvas::Canvas(Environment * environment)
: cppexpose::Object("canvas" + std::to_string(s_nextCanvasId++))
, m_environment(environment)
, m_openGLContext(nullptr)
, m_initialized(false)
, m_virtualTime(0.0f)
, m_timeDelta(0.0f)
, m_mouseDevice(cppassist::make_unique<MouseDevice>(m_environment->inputManager(), m_name))
, m_keyboardDevice(cppassist::make_unique<KeyboardDevice>(m_environment->inputManager(), m_name))
, m_replaceStage(false)
{
    // Register functions
    addFunction("getDescription",   this, &Canvas::scr_getDescription);
    addFunction("getConnections",   this, &Canvas::scr_getConnections);
    addFunction("getSlot",          this, &Canvas::scr_getSlot);
    addFunction("setSlotValue",     this, &Canvas::scr_setSlotValue);
    addFunction("createSlot",       this, &Canvas::scr_createSlot);
    addFunction("slotTypes",        this, &Canvas::scr_slotTypes);
    addFunction("createStage",      this, &Canvas::scr_createStage);
    addFunction("removeStage",      this, &Canvas::scr_removeStage);
    addFunction("createConnection", this, &Canvas::scr_createConnection);
    addFunction("removeConnection", this, &Canvas::scr_removeConnection);

    // Register canvas
    m_environment->registerCanvas(this);
}

Canvas::~Canvas()
{
    // Remove canvas
    m_environment->unregisterCanvas(this);
}

const Environment * Canvas::environment() const
{
    return m_environment;
}

Environment * Canvas::environment()
{
    return m_environment;
}

const Stage * Canvas::renderStage() const
{
    return m_renderStage.get();
}

Stage * Canvas::renderStage()
{
    return m_renderStage.get();
}

void Canvas::setRenderStage(std::unique_ptr<Stage> && stage)
{
    // Save old stage
    m_oldStage = std::unique_ptr<Stage>(m_renderStage.release());

    // Set stage
    m_renderStage = std::move(stage);

    // Issue a redraw
    m_replaceStage = true;
    redraw();
}

void Canvas::loadRenderStage(const std::string & name)
{
    // Find component
    auto component = m_environment->componentManager()->component<gloperate::Stage>(name);
    if (component)
    {
        // Create stage
        setRenderStage(std::unique_ptr<Stage>(component->createInstance(m_environment)));
    }
}

const AbstractGLContext * Canvas::openGLContext() const
{
    return m_openGLContext;
}

AbstractGLContext * Canvas::openGLContext()
{
    return m_openGLContext;
}

void Canvas::setOpenGLContext(AbstractGLContext * context)
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

void Canvas::updateTime()
{
    // In multithreaded viewers, updateTime() might get called several times
    // before render(). Therefore, the time delta is accumulated until the
    // pipeline is actually rendered, and then reset by the method render().

    // Get number of milliseconds since last call
    auto duration = m_clock.elapsed();
    m_clock.reset();

    // Determine time delta and virtual time
    float timeDelta = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
    m_virtualTime += timeDelta;
    m_timeDelta   += timeDelta;

    // Update timing
    auto slotVirtualTime = getSlot<float>(m_renderStage.get(), "virtualTime");
    if (slotVirtualTime) slotVirtualTime->setValue(m_virtualTime);
    auto slotTimeDelta = getSlot<float>(m_renderStage.get(), "timeDelta");
    if (slotTimeDelta) slotTimeDelta->setValue(m_timeDelta);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::setViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
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

void Canvas::render(globjects::Framebuffer * targetFBO)
{
    // Reset time delta
    m_timeDelta = 0.0f;

    auto fboName = targetFBO->hasName() ? targetFBO->name() : std::to_string(targetFBO->id());
    cppassist::debug(2, "gloperate") << "render(); " << "targetFBO: " << fboName;

    // Abort if not initialized
    if (!m_initialized) return;

    // Check if the render stage is to be replaced
    if (m_replaceStage)
    {
        // Check if an old stage must be destroyed
        if (m_oldStage)
        {
            // Deinitialize old stage
            m_oldStage->deinitContext(m_openGLContext);

            // Destroy old stage
            m_oldStage = nullptr;
        }

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

        // Replace finished
        m_replaceStage = false;
    }

    // Render
    auto slotTargetFBO = getSlot<globjects::Framebuffer *>(m_renderStage.get(), "targetFBO");
    if (slotTargetFBO)
    {
        slotTargetFBO->setValue(targetFBO);
        m_renderStage->process(m_openGLContext);
    }
}

void Canvas::promoteKeyPress(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "keyPressed(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyPress(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteKeyRelease(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "keyReleased(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyRelease(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseMove(const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseMoved(" << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->move(pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonPress(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonRelease(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "mouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->wheelScroll(delta, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::checkRedraw()
{
    auto slotRendered = getSlot<globjects::Framebuffer *>(m_renderStage.get(), "rendered");
    if (slotRendered && !slotRendered->isValid())
    {
        redraw();
    }
}

cppexpose::Variant Canvas::scr_getDescription(const std::string & path)
{
    Stage * stage = getStageObject(path);

    if (stage)
    {
        return stage->scr_getDescription();
    }

    return cppexpose::Variant();
}

cppexpose::Variant Canvas::scr_getSlot(const std::string & path)
{
    std::string slotName;
    Stage * stage = getStageObjectForSlot(path, slotName);

    if (stage)
    {
        return stage->scr_getSlot(slotName);
    }

    return cppexpose::Variant();
}

void Canvas::scr_setSlotValue(const std::string & path, const cppexpose::Variant & value)
{
    std::string slotName;
    Stage * stage = getStageObjectForSlot(path, slotName);

    if (stage)
    {
        stage->scr_setSlotValue(slotName, value);
    }
}

std::string Canvas::scr_createStage(const std::string & path, const std::string & className, const std::string & name)
{
    Stage * stage = getStageObject(path);

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        return pipeline->scr_createStage(className, name);
    }

    return "";
}

void Canvas::scr_removeStage(const std::string & path, const std::string & name)
{
    Stage * stage = getStageObject(path);

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        pipeline->scr_removeStage(name);
    }
}

cppexpose::Variant Canvas::scr_slotTypes(const std::string & path)
{
    Stage * stage = getStageObject(path);

    if (stage)
    {
        return stage->scr_slotTypes();
    }

    return cppexpose::Variant();
}

void Canvas::scr_createSlot(const std::string & path, const std::string & slotType, const std::string & type, const std::string & name)
{
    Stage * stage = getStageObject(path);

    if (stage)
    {
        return stage->scr_createSlot(slotType, type, name);
    }
}

cppexpose::Variant Canvas::scr_getConnections(const std::string & path)
{
    Stage * stage = getStageObject(path);

    if (stage)
    {
        return stage->scr_getConnections();
    }

    return cppexpose::Variant();
}

void Canvas::scr_createConnection(const std::string & from, const std::string & to)
{
    Stage * stage = getStageObject("");

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);
        pipeline->scr_createConnection(from, to);
    }
}

void Canvas::scr_removeConnection(const std::string & to)
{
    Stage * stage = getStageObject("");

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);
        pipeline->scr_removeConnection(to);
    }
}

Stage * Canvas::getStageObject(const std::string & path) const
{
    // Begin with render stage
    Stage * stage = m_renderStage.get();
    if (!stage) return nullptr;

    // Split path
    auto names = string::split(path, '.');

    // Resolve path
    for (auto name : names)
    {
        // Abort if stage does not have sub-stages
        if (!stage->isPipeline()) {
            return nullptr;
        }

        // Convert to pipeline
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        // Get sub-stage
        if (!(stage == m_renderStage.get() && name == m_renderStage->name()))
        {
            stage = pipeline->stage(name);
            if (!stage) return nullptr;
        }
    }

    // Return stage
    return stage;
}

Stage * Canvas::getStageObjectForSlot(const std::string & path, std::string & slotName) const
{
    // Begin with render stage
    Stage * stage = m_renderStage.get();
    if (!stage) return nullptr;

    // Split path
    auto names = string::split(path, '.');
    if (names.size() == 0) return nullptr;

    // Remove name of slot
    slotName = names[names.size() - 1];
    names.pop_back();

    // Resolve path
    for (auto name : names)
    {
        // Abort if stage does not have sub-stages
        if (!stage->isPipeline()) {
            return nullptr;
        }

        // Convert to pipeline
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        // Get sub-stage
        if (!(stage == m_renderStage.get() && name == m_renderStage->name()))
        {
            stage = pipeline->stage(name);
            if (!stage) return nullptr;
        }
    }

    // Return stage
    return stage;
}


} // namespace gloperate
