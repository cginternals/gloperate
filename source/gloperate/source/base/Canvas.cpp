
#include <gloperate/base/Canvas.h>

#include <glm/glm.hpp>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>
#include <cppassist/string/manipulation.h>

#include <cppexpose/variant/Variant.h>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Slot.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>


using namespace cppexpose;


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
    addFunction("getSlotTypes",     this, &Canvas::scr_getSlotTypes);
    addFunction("createStage",      this, &Canvas::scr_createStage);
    addFunction("removeStage",      this, &Canvas::scr_removeStage);
    addFunction("createSlot",       this, &Canvas::scr_createSlot);
    addFunction("getConnections",   this, &Canvas::scr_getConnections);
    addFunction("createConnection", this, &Canvas::scr_createConnection);
    addFunction("removeConnection", this, &Canvas::scr_removeConnection);
    addFunction("getStage",         this, &Canvas::scr_getStage);
    addFunction("getSlot",          this, &Canvas::scr_getSlot);
    addFunction("getValue",         this, &Canvas::scr_getValue);
    addFunction("setValue",         this, &Canvas::scr_setValue);

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
    std::lock_guard<std::mutex> lock(this->m_mutex);

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
    std::lock_guard<std::mutex> lock(this->m_mutex);

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
    std::lock_guard<std::mutex> lock(this->m_mutex);

    // Store viewport information
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;
    m_initialized = true;

    // Promote new viewport
    auto slotDeviceViewport = getSlot<glm::vec4>(m_renderStage.get(), "deviceViewport");
    if (slotDeviceViewport) slotDeviceViewport->setValue(m_deviceViewport);
    auto slotVirtualViewport = getSlot<glm::vec4>(m_renderStage.get(), "virtualViewport");
    if (slotVirtualViewport) slotVirtualViewport->setValue(m_virtualViewport);

    // Check if a redraw is required
    checkRedraw();
}

const glm::vec4 & Canvas::deviceViewport() const
{
    return m_deviceViewport;
}

const glm::vec4 & Canvas::virtualViewport() const
{
    return m_virtualViewport;
}

void Canvas::render(globjects::Framebuffer * targetFBO)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

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
        m_renderStage->process();
    }
}

void Canvas::promoteKeyPress(int key, int modifier)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    cppassist::debug(2, "gloperate") << "keyPressed(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyPress(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteKeyRelease(int key, int modifier)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    cppassist::debug(2, "gloperate") << "keyReleased(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyRelease(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseMove(const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    cppassist::debug(2, "gloperate") << "mouseMoved(" << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->move(pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMousePress(int button, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    cppassist::debug(2, "gloperate") << "mousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonPress(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseRelease(int button, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    cppassist::debug(2, "gloperate") << "mouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonRelease(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

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

cppexpose::Variant Canvas::scr_getSlotTypes(const std::string & path)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage)
    {
        Variant types = Variant::array();

        types.asArray()->push_back("bool");
        types.asArray()->push_back("int");
        types.asArray()->push_back("float");
        types.asArray()->push_back("vec2");
        types.asArray()->push_back("vec3");
        types.asArray()->push_back("vec4");
        types.asArray()->push_back("ivec2");
        types.asArray()->push_back("ivec3");
        types.asArray()->push_back("ivec4");
        types.asArray()->push_back("string");
        types.asArray()->push_back("file");
        types.asArray()->push_back("color");
        types.asArray()->push_back("texture");
        types.asArray()->push_back("fbo");

        return types;
    }

    return cppexpose::Variant();
}

std::string Canvas::scr_createStage(const std::string & path, const std::string & name, const std::string & type)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        // Get component manager
        auto componentManager = m_environment->componentManager();

        // Get component for the requested stage
        auto component = componentManager->component<gloperate::Stage>(type);
        if (component)
        {
            // Create stage
            auto stage = component->createInstance(m_environment, name);
            auto stagePtr = stage.get();

            pipeline->addStage(std::move(stage));

            return stagePtr->name();
        }
    }

    return "";
}

void Canvas::scr_removeStage(const std::string & path, const std::string & name)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage && stage->isPipeline())
    {
        Pipeline * pipeline = static_cast<Pipeline*>(stage);

        Stage * targetStage = pipeline->stage(name);
        if (targetStage)
        {
            pipeline->removeStage(targetStage);
        }
    }
}

void Canvas::scr_createSlot(const std::string & path, const std::string & slot, const std::string & slotType, const std::string & type)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage)
    {
        stage->createSlot(slotType, type, slot);
    }
}

cppexpose::Variant Canvas::scr_getConnections(const std::string & path)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage)
    {
        Variant obj = Variant::array();

        auto addSlot = [&obj, this] (AbstractSlot * slot)
        {
            // Check if slot is connected
            if (slot->isConnected())
            {
                // Get connection info
                std::string from = slot->source()->qualifiedName();
                std::string to   = slot->qualifiedName();

                // Describe connection
                Variant connection = Variant::map();
                (*connection.asMap())["from"] = from;
                (*connection.asMap())["to"]   = to;

                // Add connection
                obj.asArray()->push_back(connection);
            }
        };

        // List connections
        for (auto input : stage->m_inputs)
        {
            addSlot(input);
        }

        for (auto output : stage->m_outputs)
        {
            addSlot(output);
        }

        // Return connections
        return obj;
    }

    return cppexpose::Variant();
}

void Canvas::scr_createConnection(const std::string & sourcePath, const std::string & sourceSlot, const std::string & destPath, const std::string & destSlot)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * sourceStage = getStageObject(sourcePath);
    Stage * destStage   = getStageObject(destPath);

    if (sourceStage && destStage)
    {
        AbstractSlot * slotFrom = sourceStage->getSlot(sourceSlot);
        AbstractSlot * slotTo   = destStage->getSlot(destSlot);

        if (slotFrom && slotTo)
        {
            slotTo->connect(slotFrom);
        }
    }
}

void Canvas::scr_removeConnection(const std::string & path, const std::string & slot)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);

    if (stage)
    {
        AbstractSlot * slotTo = stage->getSlot(slot);

        if (slotTo)
        {
            slotTo->disconnect();
        }
    }
}

cppexpose::Variant Canvas::scr_getStage(const std::string & path)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    // Get stage
    Stage * stage = getStageObject(path);
    if (stage)
    {
        // Compose stage information
        Variant obj = Variant::map();

        (*obj.asMap())["name"] = name();

        // List inputs
        Variant inputs = Variant::array();
        for (auto input : stage->inputs())
        {
            inputs.asArray()->push_back(input->name());
        }

        (*obj.asMap())["inputs"] = inputs;

        // List outputs
        Variant outputs = Variant::array();
        for (auto output : stage->outputs())
        {
            outputs.asArray()->push_back(output->name());
        }

        (*obj.asMap())["outputs"] = outputs;

        // List stages
        Variant stages = Variant::array();
        if (stage->isPipeline())
        {
            Pipeline * pipeline = static_cast<Pipeline*>(stage);

            auto stageList = pipeline->stages();
            for (auto stage : stageList)
            {
                stages.asArray()->push_back(stage->name());
            }
        }

        (*obj.asMap())["stages"] = stages;

        // Return information about stage
        return obj;
    }

    return cppexpose::Variant();
}

cppexpose::Variant Canvas::scr_getSlot(const std::string & path, const std::string & slotName)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Variant status = Variant::map();

    // Get stage
    Stage * stage = getStageObject(path);
    if (stage)
    {
        // Get slot
        AbstractSlot * slot = stage->getSlot(slotName);
        if (slot)
        {
            // Compose slot information
            (*status.asMap())["name"]  = slot->name();
            (*status.asMap())["type"]  = slot->typeName();
            (*status.asMap())["value"] = slot->toVariant();
            // [TODO] input->options()
        }
    }

    return status;
}

cppexpose::Variant Canvas::scr_getValue(const std::string & path, const std::string & slotName)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);
    if (stage)
    {
        AbstractSlot * slot = stage->getSlot(slotName);
        if (slot)
        {
            return slot->toVariant();
        }
    }

    return cppexpose::Variant();
}

void Canvas::scr_setValue(const std::string & path, const std::string & slotName, const cppexpose::Variant & value)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    Stage * stage = getStageObject(path);
    if (stage)
    {
        AbstractSlot * slot = stage->getSlot(slotName);
        if (slot)
        {
            slot->fromVariant(value);
        }
    }
}

Stage * Canvas::getStageObject(const std::string & path) const
{
    // Begin with empty stage
    Stage * stage = nullptr;

    // Split path
    auto names = string::split(path, '.');

    // Resolve path
    for (auto name : names)
    {
        if (!stage && name == "root")
        {
            stage = m_renderStage.get();
        }

        else if (stage && stage->isPipeline())
        {
            Pipeline * pipeline = static_cast<Pipeline*>(stage);

            stage = pipeline->stage(name);
            if (!stage) return nullptr;
        }

        else
        {
            return nullptr;
        }
    }

    // Return stage
    return stage;
}


} // namespace gloperate
