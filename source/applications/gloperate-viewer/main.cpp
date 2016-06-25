
#include <iostream>

#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QQmlEngine>
#include <QQmlContext>

#include <cppassist/string/conversion.h>

#include <cppexpose/scripting/ScriptContext.h>
#include <cppexpose/reflection/Property.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/Application.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/base/QuickView.h>
#include <gloperate-qtquick/scripting/QmlEngine.h>
#include <gloperate-qtquick/scripting/QmlScriptContext.h>

#include "Config.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Determine data paths
    const auto qmlPath = QString::fromStdString(gloperate::dataPath()) + "/gloperate/qml";

    // Create gloperate environment
    Environment environment;

    // Initialize Qt application
    gloperate_qt::Application app(&environment, argc, argv);
    const auto fi = QFileInfo(QCoreApplication::applicationFilePath());

    QApplication::setApplicationDisplayName(fi.baseName());
    QApplication::setApplicationName(GLOPERATE_PROJECT_NAME);
    QApplication::setApplicationVersion(GLOPERATE_VERSION);
    QApplication::setOrganizationName(GLOPERATE_AUTHOR_ORGANIZATION);
    QApplication::setOrganizationDomain(GLOPERATE_AUTHOR_DOMAIN);

    // Load configuration
    Config config(environment);

    // Configure update manager
    UpdateManager updateManager(&environment);

    // Create QML engine
    QmlEngine qmlEngine(&environment);
    qmlEngine.addImportPath(qmlPath);
    qmlEngine.rootContext()->setContextProperty("config", &config);

    // Create scripting context backend
    environment.setupScripting(
        new gloperate_qtquick::QmlScriptContext(&qmlEngine)
    );

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins("loaders");
    environment.componentManager()->scanPlugins("stages");
    environment.componentManager()->scanPlugins("exporter");

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
