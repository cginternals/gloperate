
#include <gloperate/scripting/ScriptEnvironment.h>

#include <cppassist/logging/logging.h>

#include <cppexpose/reflection/Object.h>

#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/scripting/SystemApi.h>
#include <gloperate/scripting/TimerApi.h>
#include <gloperate/scripting/ComponentsApi.h>


namespace gloperate
{


ScriptEnvironment::ScriptEnvironment(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
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

ScriptEnvironment::~ScriptEnvironment()
{
}

void ScriptEnvironment::setupScripting(const std::string & backendName)
{
    m_apis.clear();

    m_scriptContext.reset(new cppexpose::ScriptContext(
        backendName.length() > 0 ? backendName : "javascript"
    ) );

    initialize();
}

void ScriptEnvironment::setupScripting(cppexpose::AbstractScriptBackend * backend)
{
    m_apis.clear();

    m_scriptContext.reset(new cppexpose::ScriptContext(backend));

    initialize();
}

const cppexpose::ScriptContext * ScriptEnvironment::scriptContext() const
{
    return m_scriptContext.get();
}

cppexpose::ScriptContext * ScriptEnvironment::scriptContext()
{
    return m_scriptContext.get();
}

void ScriptEnvironment::addApi(cppexpose::Object * api)
{
    // Add script API to list
    m_apis.push_back(api);

    // Connect object to scripting engine
    m_scriptContext->registerObject(api);
}

void ScriptEnvironment::removeApi(cppexpose::Object * api)
{
    // Unregister object from scripting engine
    m_scriptContext->unregisterObject(api);
}

void ScriptEnvironment::setHelpText(const std::string & text)
{
    m_helpText = text;
}

cppexpose::Variant ScriptEnvironment::execute(const std::string & code)
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

void ScriptEnvironment::initialize()
{
    // There must be a valid scripting context
    if (!m_scriptContext.get()) {
        return;
    }

    // Use the global namespace 'gloperate'
    m_scriptContext->setGlobalNamespace("gloperate");

    // Output scripting errors to console
    m_scriptContext->scriptException.connect( [] (const std::string & error) -> void
    {
        cppassist::critical() << "Scripting Error: " << error;
    });

    // Register default scripting APIs
    m_systemApi.reset(new SystemApi(m_viewerContext));
    addApi(m_systemApi.get());

    m_timerApi.reset(new TimerApi(m_viewerContext));
    addApi(m_timerApi.get());

    m_componentsApi.reset(new ComponentsApi(m_viewerContext));
    addApi(m_componentsApi.get());
}


} // namespace gloperate
