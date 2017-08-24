
#include <gloperate/base/Environment.h>

#include <algorithm>

#include <cppassist/memory/make_unique.h>
#include <cppassist/logging/logging.h>

#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/base/Canvas.h>


namespace gloperate
{


Environment::Environment()
: cppexpose::Object("gloperate")
, m_componentManager()
, m_resourceManager(this)
, m_system(this)
, m_inputManager(this)
, m_timerManager(this)
, m_scriptContext(nullptr)
, m_safeMode(false)
{
    addProperty(&m_componentManager);
    addProperty(&m_resourceManager);
    addProperty(&m_system);
    addProperty(&m_inputManager);
    addProperty(&m_timerManager);
}

Environment::~Environment()
{
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

const InputManager * Environment::inputManager() const
{
    return &m_inputManager;
}

InputManager * Environment::inputManager()
{
    return &m_inputManager;
}

const TimerManager * Environment::timerManager() const
{
    return &m_timerManager;
}

TimerManager * Environment::timerManager()
{
    return &m_timerManager;
}

const std::vector<Canvas *> & Environment::canvases() const
{
    return m_canvases;
}

std::vector<Canvas *> Environment::canvases()
{
    return m_canvases;
}

const cppexpose::ScriptContext * Environment::scriptContext() const
{
    return m_scriptContext.get();
}

cppexpose::ScriptContext * Environment::scriptContext()
{
    return m_scriptContext.get();
}

void Environment::setupScripting(const std::string & backendName)
{
    initializeScripting(cppassist::make_unique<cppexpose::ScriptContext>(
        backendName.length() > 0 ? backendName : "javascript"
    ) );
}

void Environment::setupScripting(std::unique_ptr<cppexpose::AbstractScriptBackend>  && backend)
{
    initializeScripting(cppassist::make_unique<cppexpose::ScriptContext>(std::move(backend)));
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

void Environment::exit(int exitCode)
{
    // Emit signal
    this->exitApplication(exitCode);
}

bool Environment::safeMode()
{
    return m_safeMode;
}

void Environment::setSafeMode(bool safeMode)
{
    m_safeMode = safeMode;
}

void Environment::initializeScripting(std::unique_ptr<cppexpose::ScriptContext> && scriptContext)
{
    // Check parameters
    if (!scriptContext)
    {
        return;
    }

    // Set new script context
    m_scriptContext = std::move(scriptContext);

    // Set global object
    m_scriptContext->addGlobalObject(this);

    // Output scripting errors to console
    m_scriptContext->scriptException.connect( [] (const std::string & error) -> void
    {
        cppassist::critical() << "Scripting Error: " << error;
    });

    // Set help text
    m_helpText =
        "Available commands:\n"
        "  help: Print this help message\n"
        "  exit: Exit the application\n";
}

void Environment::registerCanvas(Canvas * canvas)
{
    m_canvases.push_back(canvas);
    addProperty(canvas);
}

void Environment::unregisterCanvas(Canvas * canvas)
{
    removeProperty(canvas);
    m_canvases.erase(std::find(m_canvases.begin(), m_canvases.end(), canvas));
}


} // namespace gloperate
