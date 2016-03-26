
#include <QApplication>
#include <QMainWindow>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>
#include <gloperate-qt/viewer/RenderWindow.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    // Create viewer context
    ViewerContext viewerContext;

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
