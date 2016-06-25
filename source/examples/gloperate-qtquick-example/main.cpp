
#include <QApplication>
#include <QQmlEngine>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/Application.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/base/QuickView.h>
#include <gloperate-qtquick/scripting/QmlEngine.h>
#include <gloperate-qtquick/scripting/QmlScriptContext.h>


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create gloperate environment
    Environment environment;

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins("loaders");
    environment.componentManager()->scanPlugins("stages");

    // Initialize Qt application
    gloperate_qt::Application app(&environment, argc, argv);
    UpdateManager updateManager(&environment);

    // Create QML engine
    QmlEngine qmlEngine(&environment);
    qmlEngine.addImportPath(qmlPath);

    // Create scripting context backend
    environment.setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

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
