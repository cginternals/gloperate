
#include <QApplication>
#include <QQmlEngine>

#include <cppexpose/scripting/example/TreeNode.h>

#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QmlScriptContext.h>
#include <gloperate-qtquick/viewer/QuickView.h>


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create viewer context
    ViewerContext viewerContext;

    // Configure and load plugins
    viewerContext.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    viewerContext.componentManager()->scanPlugins("loaders");
    viewerContext.componentManager()->scanPlugins("stages");

    // Initialize Qt application
    gloperate_qt::Application app(&viewerContext, argc, argv);
    UpdateManager updateManager(&viewerContext);

    // Create QML engine
    QmlEngine qmlEngine(&viewerContext);
    qmlEngine.addImportPath(qmlPath);

    // Create scripting context backend
    viewerContext.scriptEnvironment()->setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

    // [DEBUG]
    cppexpose::TreeNode tree("tree");
    viewerContext.scriptEnvironment()->addApi(&tree);

    // Load and show QML
    QuickView * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl::fromLocalFile(qmlPath + "/ExampleViewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    int res = app.exec();

    // Clean up
    delete window;
    return res;
}
