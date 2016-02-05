
#include <iostream>

#include <QApplication>
#include <QMainWindow>

#include <globjects/base/baselogging.h>

//#include <gloperate/plugin/PluginManager.h>
//#include <gloperate/plugin/PainterPlugin.h>
//#include <gloperate/resources/ResourceManager.h>

#include <gloperate/viewer/ViewerContext.h>

#include <gloperate-qt/viewer/RenderWindow.h>
//#include <gloperate-qt/viewer/QtTextureLoader.h>
//#include <gloperate-qt/viewer/QtTextureStorer.h>
//#include <gloperate-qt/viewer/QtKeyEventProvider.h>
//#include <gloperate-qt/viewer/QtMouseEventProvider.h>
//#include <gloperate-qt/viewer/QtWheelEventProvider.h>

//#include "QtViewerMapping.h"


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    // Create viewer context
    ViewerContext viewerContext;

    /*
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());
    resourceManager.addStorer(new QtTextureStorer());

    PluginManager pluginManager;
    pluginManager.addSearchPath(QCoreApplication::applicationDirPath().toStdString());
    pluginManager.addSearchPath(QCoreApplication::applicationDirPath().toStdString() + "/plugins");
    #ifdef NDEBUG
        pluginManager.scan("painters");
    #else
        pluginManager.scan("paintersd");
    #endif

    // Choose a painter
    std::unique_ptr<gloperate::Painter> painter(nullptr);

    std::string name = (argc > 1) ? argv[1] : "PostprocessingPipeline";

    Plugin * plugin = pluginManager.plugin(name);
    if (!plugin)
    {
        globjects::fatal() << "Plugin '" << name << "' not found. Listing plugins found:";
        pluginManager.printPlugins();

        return 1;
    }

    AbstractPainterPlugin * painterPlugin = dynamic_cast<AbstractPainterPlugin *>(plugin);
    if (!painterPlugin) 
    {
        globjects::fatal() << "Plugin '" << name << "' is not a painter plugin.";

        return 1;
    }

    painter.reset(painterPlugin->createPainter(resourceManager));

    // Create Event Provider
    QtKeyEventProvider * keyProvider = new QtKeyEventProvider();
    QtMouseEventProvider * mouseProvider = new QtMouseEventProvider();
    QtWheelEventProvider * wheelProvider = new QtWheelEventProvider();
    */

    // Create OpenGL window
    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(16);

    RenderWindow * window = new RenderWindow(&viewerContext, format);

    /*
    // Put painter into window
    window->setPainter(painter.get());
    window->installEventFilter(keyProvider);
    window->installEventFilter(mouseProvider);
    window->installEventFilter(wheelProvider);
    
    // Create Mapping
    QtViewerMapping * mapping = new QtViewerMapping(window);
    mapping->setPainter(painter.get());
    mapping->addProvider(keyProvider);
    mapping->addProvider(mouseProvider);
    mapping->addProvider(wheelProvider);
    */

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setCentralWidget(QWidget::createWindowContainer(window));
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);
    mainWindow.show();

    // Run main loop
    return app.exec();
}
