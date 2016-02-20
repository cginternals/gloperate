
#include <QApplication>
#include <QQmlEngine>

#include <globjects/base/baselogging.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/stages/DemoStage.h>

#include <gloperate-qt/viewer/GLContext.h>

#include <gloperate-qtquick/QuickView.h>


using namespace gloperate;


int main(int argc, char * argv[])
{
    // Initialize Qt application
    QApplication app(argc, argv);

    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

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

    // Run main loop
    return app.exec();
}
