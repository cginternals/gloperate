 
#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QQmlEngine>
#include <QQmlContext>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/scripting/ScriptEnvironment.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>

#include <cppexpose/reflection/Object.h>

#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/QmlScriptContext.h>
#include <gloperate-qtquick/viewer/RenderItem.h>


#include "Config.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Determine data paths
    const auto qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create viewer context
    ViewerContext viewerContext;

    // Initialize Qt application
    gloperate_qt::Application app(&viewerContext, argc, argv);
    const auto fi = QFileInfo(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());
    QApplication::setApplicationName(GLOPERATE_PROJECT_NAME);
    QApplication::setApplicationVersion(GLOPERATE_VERSION);
    QApplication::setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);

    // Load configuration
    Config config(viewerContext);

    // Configure update manager
    UpdateManager updateManager(&viewerContext);

    // Create QML engine
    QmlEngine qmlEngine(&viewerContext);
    qmlEngine.addImportPath(qmlPath);
    qmlEngine.rootContext()->setContextProperty("config", &config);

    // Create scripting context backend
    viewerContext.scriptEnvironment()->setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

    // Configure and load plugins
    viewerContext.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    viewerContext.componentManager()->scanPlugins("loaders");
    viewerContext.componentManager()->scanPlugins("stages");
    viewerContext.componentManager()->scanPlugins("exporter");

    // Load and show QML
    auto * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl::fromLocalFile(qmlPath + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    int res = app.exec();

    // Clean up
    delete window;
    return res;
}
