
#include <QApplication>
#include <QQmlEngine>

#include <globjects/base/baselogging.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/stages/DemoStage.h>

#include <gloperate-qtquick/QuickView.h>


using namespace gloperate;


int main(int argc, char * argv[])
{
    // Initialize Qt application
    QApplication app(argc, argv);

    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Enable alpha buffer
    QQuickWindow::setDefaultAlphaBuffer(true);

    // Create viewer context
    ViewerContext viewerContext;

    // Create render stage
    DemoStage * renderStage = new DemoStage(&viewerContext);

    // Load and show QML
    gloperate_qtquick::QuickView * window = new gloperate_qtquick::QuickView(
        &viewerContext,
        renderStage
    );
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->engine()->addImportPath(qmlPath);
    window->setSource(QUrl(qmlPath + "/Main.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    /*
    // Initialize context, print context info
    window->context()->use();
//  window->context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
    globjects::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    window->context()->release();
    */

    // Run main loop
    return app.exec();
}