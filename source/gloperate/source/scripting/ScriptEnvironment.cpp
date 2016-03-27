
#include <gloperate/scripting/ScriptEnvironment.h>

#include <globjects/base/baselogging.h>

#include <reflectionzeug/Object.h>

#include <scriptzeug/ScriptContext.h>

#include <gloperate/scripting/SystemApi.h>
#include <gloperate/scripting/TimerApi.h>
#include <gloperate/scripting/TestApi.h>


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

    m_scriptContext.reset(new scriptzeug::ScriptContext(
        backendName.length() > 0 ? backendName : "javascript"
    ) );

    initialize();
}

void ScriptEnvironment::setupScripting(scriptzeug::AbstractScriptContext * backend)
{
    m_apis.clear();

    m_scriptContext.reset(new scriptzeug::ScriptContext(backend));

    initialize();
}

const scriptzeug::ScriptContext * ScriptEnvironment::scriptContext() const
{
    return m_scriptContext.get();
}

scriptzeug::ScriptContext * ScriptEnvironment::scriptContext()
{
    return m_scriptContext.get();
}

void ScriptEnvironment::addApi(reflectionzeug::Object * api)
{
    // Add script API to list
    m_apis.push_back(api);

    // Connect object to scripting engine
    m_scriptContext->registerObject(api);
}

void ScriptEnvironment::removeApi(reflectionzeug::Object * api)
{
    // Unregister object from scripting engine
    m_scriptContext->unregisterObject(api);
}

void ScriptEnvironment::setHelpText(const std::string & text)
{
    m_helpText = text;
}

reflectionzeug::Variant ScriptEnvironment::execute(const std::string & code)
{
    // Substitute shortcut commands
    std::string cmd = code;
    if (cmd == "help") {
        // Print help text
        globjects::info() << m_helpText;
        return reflectionzeug::Variant();
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

    // Output scripting errors to console
    m_scriptContext->scriptException.connect( [] (const std::string & error) -> void
    {
        globjects::critical() << "Scripting Error: " << error;
    });

    // Register default scripting APIs
    m_systemApi.reset(new SystemApi(m_viewerContext));
    addApi(m_systemApi.get());

    m_timerApi.reset(new TimerApi(m_viewerContext));
    addApi(m_timerApi.get());

    // Register test API
    TestApi * test1 = new TestApi("test1", 23);
    TestApi * test2 = new TestApi("test2", 42);
    test1->addProperty(test2);
    addApi(test1);
}


} // namespace gloperate
