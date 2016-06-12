
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
, m_timeManager(this)
, m_inputManager()
, m_resourceManager(this)
, m_tree("tree")
{
    registerLocalPlugins();

    addProperty(&m_tree);
}

ViewerContext::~ViewerContext()
{
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

const cppexpose::ScriptContext * ViewerContext::scriptContext() const
{
    return m_scriptContext.get();
}

cppexpose::ScriptContext * ViewerContext::scriptContext()
{
    return m_scriptContext.get();
}

const cppexpose::ComponentManager * ViewerContext::componentManager() const
{
    return &m_componentManager;
}

cppexpose::ComponentManager * ViewerContext::componentManager()
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

const std::vector<Surface *> & ViewerContext::surfaces() const
{
    return m_surfaces;
}

std::vector<Surface *> & ViewerContext::surfaces()
{
    return m_surfaces;
}

void ViewerContext::setupScripting(const std::string & backendName)
{
//  m_apis.clear();

    m_scriptContext.reset(new cppexpose::ScriptContext(
        backendName.length() > 0 ? backendName : "javascript"
    ) );

    m_scriptContext->setGlobalObject(this);

    initializeScripting();
}

void ViewerContext::setupScripting(cppexpose::AbstractScriptBackend * backend)
{
//  m_apis.clear();

    m_scriptContext.reset(new cppexpose::ScriptContext(backend));

    m_scriptContext->setGlobalObject(this);

    initializeScripting();
}

cppexpose::Variant ViewerContext::executeScript(const std::string & code)
{
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

void ViewerContext::initializeScripting()
{
    // There must be a valid scripting context
    if (!m_scriptContext.get()) {
        return;
    }

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

    /*
    // Register default scripting APIs
    m_systemApi.reset(new SystemApi(m_viewerContext));
    addApi(m_systemApi.get());

    m_timerApi.reset(new TimerApi(m_viewerContext));
    addApi(m_timerApi.get());

    m_componentsApi.reset(new ComponentsApi(m_viewerContext));
    addApi(m_componentsApi.get());

    m_pipelineApi.reset(new PipelineApi(m_viewerContext));
    addApi(m_pipelineApi.get());
    */
}

void ViewerContext::registerSurface(Surface * surface)
{
    m_surfaces.push_back(surface);
}

void ViewerContext::unregisterSurface(Surface * surface)
{
    m_surfaces.erase(std::find(m_surfaces.begin(), m_surfaces.end(), surface));
}


} // namespace gloperate
