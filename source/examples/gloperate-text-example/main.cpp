
#include <QApplication>

#include <cppexpose/plugin/ComponentManager.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>

#include <gloperate-qt/base/Application.h>
#include <gloperate-qt/base/UpdateManager.h>

#include <gloperate-qtquick/QuickView.h>
#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QmlScriptContext.h>

#include "TextRenderingPipeline.h"


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
    environment.setupScripting(cppassist::make_unique<gloperate_qtquick::QmlScriptContext>(&qmlEngine));

    // Load and show QML
    QuickView window(&qmlEngine);
    window.setResizeMode(QQuickView::SizeRootObjectToView);
    window.setSource(QUrl::fromLocalFile(qmlEngine.gloperateModulePath() + "/TextExampleViewer.qml"));
    window.setGeometry(100, 100, 1280, 720);
    window.show();

    // Run main loop
    return app.exec();
}
