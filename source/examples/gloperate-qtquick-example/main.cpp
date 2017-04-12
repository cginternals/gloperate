
#include <QApplication>
#include <QQmlEngine>

#include <cppassist/cmdline/ArgumentParser.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>
#include <gloperate-qt/base/Application.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/QuickView.h>
#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QmlScriptContext.h>


using namespace gloperate;
using namespace gloperate_qt;
using namespace gloperate_qtquick;


int main(int argc, char * argv[])
{
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

    // Create scripting context backend
    environment.setupScripting(
        cppassist::make_unique<gloperate_qtquick::QmlScriptContext>(&qmlEngine)
    );

    // Load and show QML
    QuickView window(&qmlEngine);
    // Specify desired context format
    cppassist::ArgumentParser argumentParser;
    argumentParser.parse(argc, argv);
    const auto contextString = argumentParser.value("--context");
    if(!contextString.empty())
    {
        gloperate::GLContextFormat format;
        if(!format.initializeFromString(contextString))
            return 1;
        QSurfaceFormat qFormat = gloperate_qt::GLContextFactory::toQSurfaceFormat(format);
        window.setFormat(qFormat);
    }
    window.setResizeMode(QQuickView::SizeRootObjectToView);
    window.setSource(QUrl::fromLocalFile(qmlEngine.gloperateModulePath() + "/ExampleViewer.qml"));
    window.setGeometry(100, 100, 1280, 720);
    window.show();

    // Run main loop
    return app.exec();
}
