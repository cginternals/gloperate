
#include <gloperate/base/Canvas.h>

#include <functional>
#include <algorithm>

#include <glm/glm.hpp>

#include <glbinding/gl/enum.h>

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
#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/DepthStencilRenderTarget.h>
#include <gloperate/rendering/AttachmentType.h>
#include <gloperate/stages/base/BlitStage.h>


using namespace cppassist;
using namespace cppexpose;


namespace
{


template <typename T>
gloperate::Input<T> * getInput(gloperate::Stage * stage, std::function<bool(gloperate::Input<T> *)> callback)
{
    if (!stage)
    {
        return nullptr;
    }

    const auto & inputs = stage->inputs();

    const auto it = std::find_if(inputs.begin(), inputs.end(), [callback](gloperate::AbstractSlot * slot) {
        const auto input = dynamic_cast<gloperate::Input<T> *>(slot);

        if (!input)
        {
            return false;
        }

        return callback(input);
    });

    if (it == inputs.end())
    {
        return nullptr;
    }

    return static_cast<gloperate::Input<T> *>(*it);
}

template <typename T>
void forAllInputs(gloperate::Stage * stage, std::function<void(gloperate::Input<T> *)> callback)
{
    if (!stage)
    {
        return;
    }

    const auto & inputs = stage->inputs();

    for (const auto input : inputs)
    {
        const auto inputT = dynamic_cast<gloperate::Input<T> *>(input);

        if (!inputT)
        {
            continue;
        }

        callback(inputT);
    }
}

template <typename T>
gloperate::Output<T> * getOutput(gloperate::Stage * stage, std::function<bool(gloperate::Output<T> *)> callback)
{
    if (!stage)
    {
        return nullptr;
    }

    const auto & outputs = stage->outputs();

    const auto it = std::find_if(outputs.begin(), outputs.end(), [callback](gloperate::AbstractSlot * slot) {
        const auto output = dynamic_cast<gloperate::Output<T> *>(slot);

        if (!output)
        {
            return false;
        }

        return callback(output);
    });

    if (it == outputs.end())
    {
        return nullptr;
    }

    return static_cast<gloperate::Output<T> *>(*it);
}

template <typename T>
void forAllOutputs(gloperate::Stage * stage, std::function<void(gloperate::Output<T> *)> callback)
{
    if (!stage)
    {
        return;
    }

    const auto & outputs = stage->outputs();

    for (const auto output : outputs)
    {
        const auto outputT = dynamic_cast<gloperate::Output<T> *>(output);

        if (!outputT)
        {
            continue;
        }

        callback(outputT);
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
, m_timeDelta(0.0f)
, m_blitStage(cppassist::make_unique<BlitStage>(environment, "FinalBlit"))
, m_mouseDevice(cppassist::make_unique<MouseDevice>(m_environment->inputManager(), m_name))
, m_keyboardDevice(cppassist::make_unique<KeyboardDevice>(m_environment->inputManager(), m_name))
, m_replaceStage(false)
, m_colorTarget(cppassist::make_unique<ColorRenderTarget>())
, m_depthTarget(cppassist::make_unique<DepthRenderTarget>())
, m_depthStencilTarget(cppassist::make_unique<DepthStencilRenderTarget>())
{
    // Register functions
    addFunction("onStageInputChanged", this, &Canvas::scr_onStageInputChanged);
    addFunction("getSlotTypes",        this, &Canvas::scr_getSlotTypes);
    addFunction("createStage",         this, &Canvas::scr_createStage);
    addFunction("removeStage",         this, &Canvas::scr_removeStage);
    addFunction("createSlot",          this, &Canvas::scr_createSlot);
    addFunction("getConnections",      this, &Canvas::scr_getConnections);
    addFunction("createConnection",    this, &Canvas::scr_createConnection);
    addFunction("removeConnection",    this, &Canvas::scr_removeConnection);
    addFunction("getStage",            this, &Canvas::scr_getStage);
    addFunction("getSlot",             this, &Canvas::scr_getSlot);
    addFunction("getValue",            this, &Canvas::scr_getValue);
    addFunction("setValue",            this, &Canvas::scr_setValue);

    // Register canvas
    m_environment->registerCanvas(this);

    m_colorTarget->setAttachmentType(AttachmentType::Color);
    m_depthTarget->setAttachmentType(AttachmentType::Depth);
    m_depthStencilTarget->setAttachmentType(AttachmentType::DepthStencil);
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

    // Connect to changes on the stage's input slots
    m_inputChangedConnection = m_renderStage->inputChanged.connect(this, &Canvas::stageInputChanged);

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
        debug(2, "gloperate") << "deinitContext()";

        if (m_renderStage)
        {
            m_renderStage->deinitContext(m_openGLContext);
        }

        m_blitStage->deinitContext(m_openGLContext);

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        debug(2, "gloperate") << "initContext()";

        m_openGLContext = context;

        if (m_renderStage)
        {
            m_renderStage->initContext(m_openGLContext);
        }

        m_blitStage->initContext(m_openGLContext);
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
    m_timeDelta += timeDelta;

    // Update timing
    auto slotTimeDelta = getInput<float>(m_renderStage.get(), [](Input<float>* input) { return input->name() == "timeDelta"; });
    if (slotTimeDelta)
    {
        slotTimeDelta->setValue(m_timeDelta);
    }

    // Check if a redraw is required
    checkRedraw();

    // Promote changed input value to scripting
    promoteChangedInputs();
}

void Canvas::setViewport(const glm::vec4 & deviceViewport)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    // Store viewport information
    m_viewport  = deviceViewport;
    m_initialized = true;

    // Promote new viewport
    auto slotViewport = getInput<glm::vec4>(m_renderStage.get(), [](Input<glm::vec4>* input) { return input->name() == "viewport"; });
    if (slotViewport) slotViewport->setValue(m_viewport);

    // Check if a redraw is required
    checkRedraw();
}

const glm::vec4 & Canvas::viewport() const
{
    return m_viewport;
}

void Canvas::render(globjects::Framebuffer * targetFBO)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    // Reset time delta
    m_timeDelta = 0.0f;

    auto fboName = targetFBO->hasName() ? targetFBO->name() : std::to_string(targetFBO->id());
    debug(2, "gloperate") << "render(); " << "targetFBO: " << fboName;

    // Abort if not initialized
    if (!m_initialized || !m_renderStage) return;

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
        auto slotViewport = getInput<glm::vec4>(m_renderStage.get(), [](Input<glm::vec4>* input) { return input->name() == "viewport"; });
        if (slotViewport) slotViewport->setValue(m_viewport);

        // Mark output as required
        forAllOutputs<ColorRenderTarget *>(m_renderStage.get(), [](Output<ColorRenderTarget *> * output) {
            output->setRequired(true);
        });

        // Replace finished
        m_replaceStage = false;
    }

    // Extract default color and depth buffer from FBO
    if (targetFBO->isDefault())
    {
        m_colorTarget->setTarget(gl::GL_BACK_LEFT);
        m_depthTarget->setTarget(gl::GL_DEPTH_ATTACHMENT);
        m_depthStencilTarget->setTarget(gl::GL_DEPTH_STENCIL_ATTACHMENT);
    }
    else
    {
        unsigned int i = 0;
        globjects::FramebufferAttachment * colorAttachment = nullptr;
        while (i < 16 && colorAttachment == nullptr)
        {
            colorAttachment = targetFBO->getAttachment(gl::GL_COLOR_ATTACHMENT0+i);
        }
        const auto depthAttachment = targetFBO->getAttachment(gl::GL_DEPTH_ATTACHMENT);
        const auto depthStencilAttachment = targetFBO->getAttachment(gl::GL_DEPTH_STENCIL_ATTACHMENT);

        if (colorAttachment)
        {
            m_colorTarget->setTarget(colorAttachment);
        }
        else
        {
            m_colorTarget->releaseTarget();
        }

        if (depthStencilAttachment)
        {
            m_depthTarget->setTarget(depthStencilAttachment);
            m_depthStencilTarget->setTarget(depthStencilAttachment);
        }
        else if (depthAttachment)
        {
            m_depthTarget->setTarget(depthAttachment);
            m_depthStencilTarget->setTarget(depthAttachment);
        }
        else
        {
            m_depthTarget->releaseTarget();
            m_depthStencilTarget->releaseTarget();
        }
    }

    // Update render stage input render targets
    forAllInputs<gloperate::ColorRenderTarget *>(m_renderStage.get(), [this](Input<ColorRenderTarget *> * input) {
        const auto renderTarget = **input;

        if (renderTarget == nullptr)
        {
            return;
        }

        input->setValue(m_colorTarget.get());
    });
    forAllInputs<gloperate::DepthRenderTarget *>(m_renderStage.get(), [this](Input<DepthRenderTarget *> * input) {
        const auto renderTarget = **input;

        if (renderTarget == nullptr)
        {
            return;
        }

        input->setValue(m_depthTarget.get());
    });
    forAllInputs<gloperate::DepthStencilRenderTarget *>(m_renderStage.get(), [this](Input<DepthStencilRenderTarget *> * input) {
        const auto renderTarget = **input;

        if (renderTarget == nullptr)
        {
            return;
        }

        input->setValue(m_depthStencilTarget.get());
    });

    // Render
    m_renderStage->process();

    auto colorOutput = getOutput<gloperate::ColorRenderTarget *>(m_renderStage.get(), [this](Output<ColorRenderTarget *> * output) {
        return **output != nullptr;
    });

    if (colorOutput)
    {
        if (**colorOutput == m_colorTarget.get())
        {

        }
        else
        {
            auto viewport = getOutput<glm::vec4>(m_renderStage.get(), [this](Output<glm::vec4> *) {
                return true;
            });

            m_blitStage->source = **colorOutput;
            m_blitStage->sourceViewport = viewport ? **viewport : m_viewport;
            m_blitStage->target = m_colorTarget.get();
            m_blitStage->targetViewport = m_viewport;
            m_blitStage->process();
        }
    }
}

void Canvas::promoteKeyPress(int key, int modifier)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "keyPressed(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyPress(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteKeyRelease(int key, int modifier)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "keyReleased(" << key << ", " << modifier << ")";

    // Promote keyboard event
    m_keyboardDevice->keyRelease(key, modifier);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseMove(const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "mouseMoved(" << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->move(pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMousePress(int button, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "mousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonPress(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseRelease(int button, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "mouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->buttonRelease(button, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::promoteMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    std::lock_guard<std::mutex> lock(this->m_mutex);

    debug(2, "gloperate") << "mouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    // Promote mouse event
    m_mouseDevice->wheelScroll(delta, pos);

    // Check if a redraw is required
    checkRedraw();
}

void Canvas::checkRedraw()
{
    bool redraw = false;
    forAllOutputs<ColorRenderTarget *>(m_renderStage.get(), [& redraw](Output<ColorRenderTarget *> * output) {
        if (**output && !output->isValid())
        {
            redraw = true;
        }
    });

    if (redraw)
    {
        this->redraw();
    }
}

void Canvas::promoteChangedInputs()
{
    std::lock_guard<std::mutex> lock(this->m_changedInputMutex);

    // Check if a callback function is set
    if (m_inputChangedCallback.isEmpty())
        return;

    // Invoke callback function for each changed input
    for (auto * slot : m_changedInputs)
    {
        // Get slot status
        std::string name = slot->name();
        Variant status = getSlotStatus("root", name);

        // Invoke callback function
        std::vector<cppexpose::Variant> params;
        params.push_back(name);
        params.push_back(status);

        m_inputChangedCallback.call(params);
    }

    // Reset
    m_changedInputs.clear();
}

void Canvas::stageInputChanged(AbstractSlot * slot)
{
    std::lock_guard<std::mutex> lock(this->m_changedInputMutex);

    // Put changed input into list, will be processed on next update
    m_changedInputs.push_back(slot);
}

void Canvas::scr_onStageInputChanged(const cppexpose::Variant & func)
{
    // Check if a function has been passed
    if (!func.hasType<cppexpose::Function>())
    {
        return;
    }

    // Save callback function
    m_inputChangedCallback = func.value<cppexpose::Function>();
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

                // Replace name of stage with "root"
                std::string stageName = m_renderStage->name();

                if (string::hasPrefix(from, stageName)) {
                    from.replace(0, stageName.length(), "root");
                }

                if (string::hasPrefix(to, stageName)) {
                    to.replace(0, stageName.length(), "root");
                }

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

    return getSlotStatus(path, slotName);
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

cppexpose::Variant Canvas::getSlotStatus(const std::string & path, const std::string & slotName)
{
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

            // Include options
            const VariantMap & options = slot->options();

            for (auto it : options)
            {
                std::string key = it.first;
                Variant & value = it.second;

                (*status.asMap())[key] = value;
            }
        }
    }

    return status;
}


} // namespace gloperate
