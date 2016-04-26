
#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/scripting/ScriptEnvironment.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>
#include <gloperate-qt/viewer/RenderWindow.h>
#include <gloperate-qt/scripting/ECMA26251SyntaxHighlighter.h>
#include <gloperate-qt/scripting/ECMA26251Completer.h>
#include <gloperate-qt/scripting/ScriptPromptWidget.h>



using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    // Create viewer context
    ViewerContext viewerContext;
    viewerContext.scriptEnvironment()->setupScripting();

    // Initialize Qt application
    gloperate_qt::Application app(&viewerContext, argc, argv);
    UpdateManager updateManager(&viewerContext);

    // Create render stage
    DemoStage * renderStage = new DemoStage(&viewerContext);

    // Create render window
    RenderWindow * window = new RenderWindow(&viewerContext);
    window->setRenderStage(renderStage);
    window->createContext();

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("gloperate viewer");
    mainWindow.resize(1280, 720);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(window));
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);
    mainWindow.show();

    // Create script console
    ScriptPromptWidget * scriptPrompt = new ScriptPromptWidget(&mainWindow);
    scriptPrompt->setSyntaxHighlighter(new ECMA26251SyntaxHighlighter);
    scriptPrompt->setCompleter(new ECMA26251Completer);
    scriptPrompt->setFrameShape(QFrame::NoFrame);
    QObject::connect(scriptPrompt, &ScriptPromptWidget::evaluate,
        [&viewerContext, scriptPrompt] (const QString & cmd)
        {
            // Execute script code
            std::string code = cmd.toStdString();
            cppexpose::Variant res = viewerContext.scriptEnvironment()->execute(code);

            // Output result
            scriptPrompt->print(QString::fromStdString(res.value<std::string>()));
        }
    );

    // Create dock window for scripting console
    QDockWidget * scriptPromptDockWidget = new QDockWidget("Script Prompt");
    scriptPromptDockWidget->setWidget(scriptPrompt);
    scriptPromptDockWidget->setObjectName("ScriptPromptWidget");
    mainWindow.addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, scriptPromptDockWidget);

    // Initialize context, print context info
    window->context()->use();
//  window->context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
    globjects::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    window->context()->release();

    // Run main loop
    return app.exec();
}
