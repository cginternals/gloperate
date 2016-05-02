
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QQmlEngine>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/scripting/ScriptContext.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/scripting/ScriptEnvironment.h>

#include <gloperate-qt/viewer/Application.h>
#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/UpdateManager.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/QmlScriptContext.h>


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


const QString SETTINGS_PLUGINS("Plugins");


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
    {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings settings;

        QStringList paths = settings.value(SETTINGS_PLUGINS).toStringList();
        for (auto path : paths)
        {
            viewerContext.componentManager()->addSearchPath(
                path.toStdString()
            );
        }
    }

    // Configure update manager
    UpdateManager updateManager(&viewerContext);

    // Create QML engine
    QmlEngine qmlEngine(&viewerContext);
    qmlEngine.addImportPath(qmlPath);

    // Create scripting context backend
    viewerContext.scriptEnvironment()->setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

    // Configure and load plugins
    viewerContext.componentManager()->addSearchPath(
        gloperate::pluginPath(), cppexpose::SearchPathType::Internal
    );
    viewerContext.componentManager()->scan("plugins");

    // Load and show QML
    QuickView * window = new QuickView(&qmlEngine);
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl(qmlPath + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    int res = app.exec();

    // Save configuration
    {
        QSettings settings;

        QStringList paths;
        for (auto path : viewerContext.componentManager()->searchPaths())
        {
            paths << QString::fromStdString(path);
        }
        settings.setValue(SETTINGS_PLUGINS, paths);
    }

    // Return
    return res;
}
