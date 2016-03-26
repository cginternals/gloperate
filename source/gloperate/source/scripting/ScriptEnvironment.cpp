
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
//  m_systemApi->setHelpText(text);
}

reflectionzeug::Variant ScriptEnvironment::execute(const std::string & code)
{
    // Substitute shortcut commands
    std::string cmd = code;
    if (cmd == "help") {
        cmd = "help()";
    } else if (cmd == "exit" || cmd == "quit") {
        cmd = "exit()";
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

    m_timerApi.reset(new TimerApi);
    addApi(m_timerApi.get());

    // Register test API
    TestApi * test1 = new TestApi("test1", 23);
    TestApi * test2 = new TestApi("test2", 42);
    test1->addProperty(test2);
    addApi(test1);

    // Connect commands that have been loaded by API functions (e.g., system.load)
    /*
    m_systemApi->command.connect([this] (const std::string & cmd) {
        // Execute code
        execute(cmd);
    } );
    */

    // Add global functions
    std::string script =
        "function help() {\n"
        "  system.help();\n"
        "}\n"
        "\n"
        "function load(filename) {\n"
        "  system.load(filename);\n"
        "}\n"
        "\n"
        "function print(value) {\n"
        "  system.print(value);\n"
        "}\n"
        "\n"
        "function exit() {\n"
        "  system.exit();\n"
        "}\n"
        "\n"
        "function quit() {\n"
        "  system.exit();\n"
        "}\n";
    execute(script);
}


} // namespace gloperate
