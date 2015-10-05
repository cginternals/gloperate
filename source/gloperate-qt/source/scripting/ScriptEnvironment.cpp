
#include <gloperate-qt/scripting/ScriptEnvironment.h>

#include <QObject>

#include <scriptzeug/ScriptContext.h>

#include <widgetzeug/ScriptPromptWidget.h>
#include <widgetzeug/ECMA26251_SyntaxHighlighter.h>
#include <widgetzeug/ECMA26251_Completer.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/scripting/SystemApi.h>
#include <gloperate-qt/scripting/SystemApi.h>
#include <gloperate-qt/scripting/TimerApi.h>


namespace gloperate_qt
{


ScriptEnvironment::ScriptEnvironment(QtOpenGLWindow * openGLWindow)
: m_openGLWindow(nullptr)
, m_scriptPrompt(nullptr)
{
    // Create JavaScript context
    m_scriptContext.reset(new scriptzeug::ScriptContext("javascript"));

    // Register default scripting APIs
    m_systemApi.reset(new SystemApi(openGLWindow));
    addScriptApi(m_systemApi.get());

    m_timerApi.reset(new TimerApi);
    addScriptApi(m_timerApi.get());

    // Connect events to scripting
    if (openGLWindow) {
        // Promote keyboard events to scripting api
        openGLWindow->installEventFilter(m_systemApi.get());

        // Promote tick events to scripting api
        openGLWindow->setTimerApi(m_timerApi.get());
    }

    // Connect commands that have been loaded by API functions (e.g., system.load)
    m_systemApi->command.connect([this] (const std::string & cmd) {
        // Execute code
        execute(cmd);
    } );

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

ScriptEnvironment::~ScriptEnvironment()
{
}

const scriptzeug::ScriptContext * ScriptEnvironment::scriptContext() const
{
    return m_scriptContext.get();
}

scriptzeug::ScriptContext * ScriptEnvironment::scriptContext()
{
    return m_scriptContext.get();
}

void ScriptEnvironment::addScriptApi(reflectionzeug::Object * api)
{
    // Add script API to list
    m_apis.push_back(api);

    // Connect object to scripting engine
    m_scriptContext->registerObject(api);

    // Register name as extra word for word completion
    if (m_scriptPrompt) {
        widgetzeug::ScriptCompleter * completer = static_cast<widgetzeug::ScriptCompleter *>(m_scriptPrompt->completer());
        if (completer) {
            completer->registerWord(QString::fromStdString(api->name()));
        }
    }
}

void ScriptEnvironment::removeScriptApi(reflectionzeug::Object * api)
{
    // Unregister object from scripting engine
    m_scriptContext->unregisterObject(api);
}

void ScriptEnvironment::connectScriptingConsole(widgetzeug::ScriptPromptWidget * console)
{
    // Check parameters
    if (!console) {
        return;
    }

    // Save scripting console
    m_scriptPrompt = console;

    // Execute commands that have been typed on the scripting console
    QObject::connect(m_scriptPrompt, &widgetzeug::ScriptPromptWidget::evaluate, [this] (const QString & cmd) {
        this->execute(cmd.toStdString());
    });

    // Connect script output (system.print)
    m_systemApi->output.connect([this] (const std::string & msg) {
        // Print message onto the scripting console
        m_scriptPrompt->print(QString::fromStdString(msg));
    });

    // Add additional auto completion words
    widgetzeug::ScriptCompleter * completer = static_cast<widgetzeug::ScriptCompleter *>(m_scriptPrompt->completer());
    if (completer) {
        // Add script API names
        for (reflectionzeug::Object * api : m_apis) {
            completer->registerWord(QString::fromStdString(api->name()));
        }

        // Add global function names
        completer->registerWords(QStringList() << "help" << "load" << "print" << "exit");
    }
}

reflectionzeug::Variant ScriptEnvironment::execute(const std::string & command)
{
    // Substitute shortcut commands
    std::string cmd = command;
    if (cmd == "help") {
        cmd = "help()";
    } else if (cmd == "exit" || cmd == "quit") {
        cmd = "exit()";
    }

    // Execute command
    return m_scriptContext->evaluate(cmd);
}

void ScriptEnvironment::setHelpText(const std::string & text)
{
    m_systemApi->setHelpText(text);
}


} // namespace gloperate_qt
