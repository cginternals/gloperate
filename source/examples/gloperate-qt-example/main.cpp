
#include <QMainWindow>
#include <QDockWidget>

#include <cppassist/logging/logging.h>
#include <cppassist/cmdline/ArgumentParser.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/Application.h>
#include <gloperate-qt/base/RenderWindow.h>
#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/scripting/ECMA26251SyntaxHighlighter.h>
#include <gloperate-qt/scripting/ECMA26251Completer.h>
#include <gloperate-qt/scripting/ScriptPromptWidget.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    // Read command line options
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);

    const auto contextString = argumentParser.value("--context");

    // Create gloperate environment
    Environment environment;
    environment.setupScripting();

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins();

    // Initialize Qt application
    gloperate_qt::Application app(&environment, argc, argv);

    // Create render window
    auto window = cppassist::make_unique<RenderWindow>(&environment);

    // Specify desired context format
    gloperate::GLContextFormat format;

    if (!contextString.empty())
    {
        if (!format.initializeFromString(contextString))
        {
            return 1;
        }
    }

    window->setContextFormat(format);

    auto windowRaw = window.get();
    window->createContext();
    window->canvas()->loadRenderStage("ShapeDemo");

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("gloperate viewer");
    mainWindow.resize(1280, 720);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(window.release())); // pass ownership to Qt
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);
    mainWindow.show();

    // Create script console
    auto scriptPrompt = cppassist::make_unique<ScriptPromptWidget>(&mainWindow);
    scriptPrompt->setSyntaxHighlighter(cppassist::make_unique<ECMA26251SyntaxHighlighter>());
    scriptPrompt->setCompleter(cppassist::make_unique<ECMA26251Completer>());
    scriptPrompt->setFrameShape(QFrame::NoFrame);

    auto scriptPromptRaw = scriptPrompt.get();
    QObject::connect(scriptPrompt.get(), &ScriptPromptWidget::evaluate,
        [&environment, scriptPromptRaw] (const QString & cmd)
        {
            // Execute script code
            std::string code = cmd.toStdString();
            cppexpose::Variant res = environment.executeScript(code);

            // Output result
            scriptPromptRaw->print(QString::fromStdString(res.value<std::string>()));
        }
    );

    // Create dock window for scripting console
    auto scriptPromptDockWidget = cppassist::make_unique<QDockWidget>("Script Prompt");
    scriptPromptDockWidget->setWidget(scriptPrompt.release());
    scriptPromptDockWidget->setObjectName("ScriptPromptWidget");
    mainWindow.addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, scriptPromptDockWidget.release());

    // Initialize context, print context info
    windowRaw->context()->use();
//  windowRaw->context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
    cppassist::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Profile:  " << GLContextUtils::profile() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    windowRaw->context()->release();

    // Run main loop
    return app.exec();
}
