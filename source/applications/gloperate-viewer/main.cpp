 
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

#ifdef USE_FFMPEG
#include <gloperate-ffmpeg/VideoTool.h>
#endif

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/QmlScriptContext.h>
#include <gloperate-qtquick/viewer/RenderItem.h>


#include "Config.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;

#ifdef USE_FFMPEG
using namespace gloperate_ffmpeg;
#endif


int main(int argc, char * argv[])
{
    // Determine data paths
    QString qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create viewer context
    ViewerContext viewerContext;

    // Initialize Qt application
    gloperate_qt::Application app(&viewerContext, argc, argv);
    const QFileInfo fi(QCoreApplication::applicationFilePath());
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

    // Load and show QML
    QuickView * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl::fromLocalFile(qmlPath + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

#ifdef USE_FFMPEG
    QQuickItem * item = window->rootObject();
    QQuickItem * qmlRenderItem = item->findChild<QQuickItem*>("renderItem");
    
    RenderItem * renderItem = static_cast<RenderItem *>(qmlRenderItem);
    RenderSurface * renderSurface = static_cast<RenderSurface *>(renderItem->surface());
    
    VideoTool * videoTool = new VideoTool();
    renderSurface->setVideoTool(videoTool);
#endif

    // Run main loop
    int res = app.exec();
    return res;
}
