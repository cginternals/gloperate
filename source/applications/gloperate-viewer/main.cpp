
#include <QQmlContext>
#include <QSurfaceFormat>

#include <cppassist/cmdline/ArgumentParser.h>

#include <cppexpose/scripting/ScriptContext.h>
#include <cppexpose/reflection/Property.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/base/Environment.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QmlScriptContext.h>

#include "Config.h"
#include "Application.h"


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
    // Read command line options
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);

    const auto contextString = argumentParser.value("--context");

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

    // Specify desired context format
    gloperate::GLContextFormat format;

    if (!contextString.empty())
    {
        if (!format.initializeFromString(contextString))
        {
            return 1;
        }
    }

    QSurfaceFormat qFormat = gloperate_qt::GLContextFactory::toQSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(qFormat);

    // Load and show QML
    qmlEngine.load(QUrl::fromLocalFile(qmlEngine.gloperateModulePath() + "/Viewer.qml"));

    // Run main loop
    return app.exec();
}
