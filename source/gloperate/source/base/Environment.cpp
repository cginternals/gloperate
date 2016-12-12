
#include <gloperate/base/Environment.h>

#include <algorithm>

#include <cppassist/logging/logging.h>

#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/base/AbstractCanvas.h>

// Local components
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/CustomFramebufferStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationStage.h>
#include <gloperate/stages/multiframe/MultiFrameControlStage.h>
#include <gloperate/stages/demos/DemoPipeline.h>
#include <gloperate/stages/demos/DemoStage.h>
#include <gloperate/stages/demos/DemoMultiFramePipeline.h>
#include <gloperate/stages/demos/SpinningRectStage.h>
#include <gloperate/stages/demos/TimerStage.h>
#include <gloperate/stages/demos/ColorizeStage.h>


namespace gloperate
{


Environment::Environment()
: cppexpose::Object("gloperate")
, m_componentManager()
, m_resourceManager(this)
, m_timeManager(this)
, m_system(this)
, m_inputManager(this)
, m_tree("tree")
, m_scriptContext(nullptr)
{
    addProperty(&m_componentManager);
    addProperty(&m_resourceManager);
    addProperty(&m_timeManager);
    addProperty(&m_system);
    addProperty(&m_inputManager);
    addProperty(&m_tree);

    registerLocalPlugins();
}

Environment::~Environment()
{
    if (m_scriptContext)
    {
        delete m_scriptContext;
    }
}

const ComponentManager * Environment::componentManager() const
{
    return &m_componentManager;
}

ComponentManager * Environment::componentManager()
{
    return &m_componentManager;
}

const ResourceManager * Environment::resourceManager() const
{
    return &m_resourceManager;
}

ResourceManager * Environment::resourceManager()
{
    return &m_resourceManager;
}

const TimeManager * Environment::timeManager() const
{
    return &m_timeManager;
}

TimeManager * Environment::timeManager()
{
    return &m_timeManager;
}

const InputManager * Environment::inputManager() const
{
    return &m_inputManager;
}

InputManager * Environment::inputManager()
{
    return &m_inputManager;
}

const std::vector<AbstractCanvas *> & Environment::canvases() const
{
    return m_canvases;
}

std::vector<AbstractCanvas *> & Environment::canvases()
{
    return m_canvases;
}

const cppexpose::ScriptContext * Environment::scriptContext() const
{
    return m_scriptContext;
}

cppexpose::ScriptContext * Environment::scriptContext()
{
    return m_scriptContext;
}

void Environment::setupScripting(const std::string & backendName)
{
    initializeScripting(new cppexpose::ScriptContext(
        backendName.length() > 0 ? backendName : "javascript"
    ) );
}

void Environment::setupScripting(cppexpose::AbstractScriptBackend * backend)
{
    initializeScripting(new cppexpose::ScriptContext(backend));
}

cppexpose::Variant Environment::executeScript(const std::string & code)
{
    // There must be a valid scripting context
    if (!m_scriptContext)
    {
        return cppexpose::Variant();
    }

    // Substitute shortcut commands
    std::string cmd = code;
    if (cmd == "help") {
        // Print help text
        cppassist::info() << m_helpText;
        return cppexpose::Variant();
    } else if (cmd == "exit" || cmd == "quit") {
        // Exit application
        cmd = "gloperate.system.exit()";
    }

    // Execute command
    return m_scriptContext->evaluate(cmd);
}

bool Environment::update()
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update();

    // Update canvas
    for (AbstractCanvas * canvas : m_canvases)
    {
        canvas->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

bool Environment::update(float delta)
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update(delta);

    // Update canvas
    for (AbstractCanvas * canvas : m_canvases)
    {
        canvas->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

void Environment::exit(int exitCode)
{
    // Emit signal
    this->exitApplication(exitCode);
}

void Environment::registerLocalPlugins()
{
    m_componentManager.addComponent(&BasicFramebufferStage::Component);
    m_componentManager.addComponent(&CustomFramebufferStage::Component);
    m_componentManager.addComponent(&TextureLoadStage::Component);
    m_componentManager.addComponent(&ProceduralTextureStage::Component);
    m_componentManager.addComponent(&MixerStage::Component);
    m_componentManager.addComponent(&SplitStage::Component);
    m_componentManager.addComponent(&MultiFrameAggregationPipeline::Component);
    m_componentManager.addComponent(&MultiFrameAggregationStage::Component);
    m_componentManager.addComponent(&MultiFrameControlStage::Component);
    m_componentManager.addComponent(&DemoPipeline::Component);
    m_componentManager.addComponent(&DemoStage::Component);
    m_componentManager.addComponent(&DemoMultiFramePipeline::Component);
    m_componentManager.addComponent(&SpinningRectStage::Component);
    m_componentManager.addComponent(&TimerStage::Component);
    m_componentManager.addComponent(&ColorizeStage::Component);
}

void Environment::initializeScripting(cppexpose::ScriptContext * scriptContext)
{
    // Check parameters
    if (!scriptContext)
    {
        return;
    }

    // Destroy old script context
    if (m_scriptContext)
    {
        delete m_scriptContext;
    }

    // Set new script context
    m_scriptContext = scriptContext;

    // Set global object
    m_scriptContext->setGlobalObject(this);

    // Output scripting errors to console
    m_scriptContext->scriptException.connect( [] (const std::string & error) -> void
    {
        cppassist::critical() << "Scripting Error: " << error;
    });

    // Set help text
    m_helpText =
        "Available commands:\n"
        "  help: Print this help message\n"
        "  exit: Exit the application\n"
        "\n"
        "APIs:\n"
        "  gloperate.system: System API (IO, keyboard handling, ...)\n"
        "  gloperate.timer:  Timer API\n"
        "\n"
        "Examples:\n"
        "  gloperate.timer.start(1000, function() { print(\"Hello Scripting World.\"); } );\n"
        "  gloperate.timer.stopAll();\n";
}

void Environment::registerCanvas(AbstractCanvas * canvas)
{
    m_canvases.push_back(canvas);
    addProperty(canvas);
}

void Environment::unregisterCanvas(AbstractCanvas * canvas)
{
    removeProperty(canvas);
    m_canvases.erase(std::find(m_canvases.begin(), m_canvases.end(), canvas));
}


} // namespace gloperate
