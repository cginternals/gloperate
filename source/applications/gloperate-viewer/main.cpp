
#include <QQmlContext>

#include <cppassist/cmdline/ArgumentParser.h>

#include <cppexpose/scripting/ScriptContext.h>
#include <cppexpose/reflection/Property.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/base/Environment.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/QuickView.h>
#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QmlScriptContext.h>

#include "Config.h"
#include "Application.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Create gloperate environment
    Environment environment;

    // Initialize Qt application
    ::Application app(&environment, argc, argv);

    // Load configuration
    Config config(environment);

    // Configure update manager
    UpdateManager updateManager(&environment);

    // Create QML engine
    QmlEngine qmlEngine(&environment);
    qmlEngine.rootContext()->setContextProperty("config", &config);

    // Create scripting context backend
    environment.setupScripting(
        cppassist::make_unique<gloperate_qtquick::QmlScriptContext>(&qmlEngine)
    );

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins("loaders");
    environment.componentManager()->scanPlugins("stages");
    environment.componentManager()->scanPlugins("exporter");

    // Load and show QML
    auto window = cppassist::make_unique<QuickView>(&qmlEngine);
    // Specify desired context format
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);
    const auto contextString = argumentParser.value("--context");
    if(!contextString.empty())
    {
        gloperate::GLContextFormat format;
        format.initializeFromString(contextString);
        QSurfaceFormat qFormat = gloperate_qt::GLContextFactory::toQSurfaceFormat(format);
        window->setFormat(qFormat);
    }
    window->setResizeMode(QQuickView::SizeRootObjectToView);
    window->setSource(QUrl::fromLocalFile(qmlEngine.gloperateModulePath() + "/Viewer.qml"));
    window->setGeometry(100, 100, 1280, 720);
    window->show();

    // Run main loop
    return app.exec();
}
