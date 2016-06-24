
#include <gloperate/viewer/ViewerContext.h>

#include <algorithm>

#include <cppassist/logging/logging.h>

#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/viewer/Surface.h>

// Local components
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/demos/DemoPipeline.h>
#include <gloperate/stages/demos/DemoStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>


namespace gloperate
{


ViewerContext::ViewerContext()
: cppexpose::Object("gloperate")
, m_componentManager(this)
, m_resourceManager(this)
, m_timeManager(this)
, m_system(this)
, m_pipeline(this)
, m_inputManager(this)
, m_tree("tree")
, m_scriptContext(nullptr)
{
    addProperty(&m_componentManager);
    addProperty(&m_resourceManager);
    addProperty(&m_timeManager);
    addProperty(&m_system);
    addProperty(&m_pipeline);
    addProperty(&m_inputManager);
    addProperty(&m_tree);

    registerLocalPlugins();
}

ViewerContext::~ViewerContext()
{
    if (m_scriptContext)
    {
        delete m_scriptContext;
    }
}

const ComponentManager * ViewerContext::componentManager() const
{
    return &m_componentManager;
}

ComponentManager * ViewerContext::componentManager()
{
    return &m_componentManager;
}

const ResourceManager * ViewerContext::resourceManager() const
{
    return &m_resourceManager;
}

ResourceManager * ViewerContext::resourceManager()
{
    return &m_resourceManager;
}

const TimeManager * ViewerContext::timeManager() const
{
    return &m_timeManager;
}

TimeManager * ViewerContext::timeManager()
{
    return &m_timeManager;
}

const InputManager * ViewerContext::inputManager() const
{
    return &m_inputManager;
}

InputManager * ViewerContext::inputManager()
{
    return &m_inputManager;
}

const std::vector<Surface *> & ViewerContext::surfaces() const
{
    return m_surfaces;
}

std::vector<Surface *> & ViewerContext::surfaces()
{
    return m_surfaces;
}

const cppexpose::ScriptContext * ViewerContext::scriptContext() const
{
    return m_scriptContext;
}

cppexpose::ScriptContext * ViewerContext::scriptContext()
{
    return m_scriptContext;
}

void ViewerContext::setupScripting(const std::string & backendName)
{
    initializeScripting(new cppexpose::ScriptContext(
        backendName.length() > 0 ? backendName : "javascript"
    ) );
}

void ViewerContext::setupScripting(cppexpose::AbstractScriptBackend * backend)
{
    initializeScripting(new cppexpose::ScriptContext(backend));
}

cppexpose::Variant ViewerContext::executeScript(const std::string & code)
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

bool ViewerContext::update()
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update();

    // Update surface
    for (Surface * surface : m_surfaces)
    {
        surface->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

bool ViewerContext::update(float delta)
{
    // Update timing and timers
    bool activeTimers = m_timeManager.update(delta);

    // Update surface
    for (Surface * surface : m_surfaces)
    {
        surface->onUpdate();
    }

    // Return indicator if any more timers are running
    return activeTimers;
}

void ViewerContext::exit(int exitCode)
{
    // Emit signal
    this->exitApplication(exitCode);
}

void ViewerContext::registerLocalPlugins()
{
    m_componentManager.addComponent(&BasicFramebufferStage::Component);
    m_componentManager.addComponent(&TextureLoadStage::Component);
    m_componentManager.addComponent(&ProceduralTextureStage::Component);
    m_componentManager.addComponent(&MixerStage::Component);
    m_componentManager.addComponent(&SplitStage::Component);
    m_componentManager.addComponent(&DemoPipeline::Component);
    m_componentManager.addComponent(&DemoStage::Component);
    m_componentManager.addComponent(&DemoTriangleStage::Component);
    m_componentManager.addComponent(&DemoTimerStage::Component);
}

void ViewerContext::initializeScripting(cppexpose::ScriptContext * scriptContext)
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

void ViewerContext::registerSurface(Surface * surface)
{
    m_surfaces.push_back(surface);
    addProperty(surface);
}

void ViewerContext::unregisterSurface(Surface * surface)
{
    removeProperty(surface);
    m_surfaces.erase(std::find(m_surfaces.begin(), m_surfaces.end(), surface));
}


} // namespace gloperate
