
#include <iostream>

#include <gloperate/ext-includes-begin.h>
#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QString>
#include <QMainWindow>
#include <QOpenGLContext>
#include <gloperate/ext-includes-end.h>

#include <globjects/base/baselogging.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/PainterPlugin.h>
#include <gloperate/resources/ResourceManager.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/viewer/QtTextureLoader.h>
#include <gloperate-qt/viewer/QtTextureStorer.h>
#include <gloperate-qt/viewer/QtKeyEventProvider.h>
#include <gloperate-qt/viewer/QtMouseEventProvider.h>
#include <gloperate-qt/viewer/QtWheelEventProvider.h>
#include <gloperate-qt/viewer/QtFrameEventProvider.h>

#include "QtViewerMapping.h"


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());
    resourceManager.addStorer(new QtTextureStorer());

    PluginManager pluginManager;
    pluginManager.addPath(QCoreApplication::applicationDirPath().toStdString());
#ifdef NDEBUG
    pluginManager.addPath("plugins");
    pluginManager.scan("painters");
#else
    pluginManager.addPath("plugins/debug");
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
    QtFrameEventProvider * frameProvider = new QtFrameEventProvider();

    // Create OpenGL window
    QSurfaceFormat format;
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(16);

    QtOpenGLWindow * window = new QtOpenGLWindow(resourceManager, format);
    window->setPainter(painter.get());
    window->installEventFilter(keyProvider);
    window->installEventFilter(mouseProvider);
    window->installEventFilter(wheelProvider);
    window->installEventFilter(frameProvider);
    
    // Create Mapping
    QtViewerMapping * mapping = new QtViewerMapping(window);
    mapping->setPainter(painter.get());
    mapping->addProvider(keyProvider);
    mapping->addProvider(mouseProvider);
    mapping->addProvider(wheelProvider);
    

    QRect rect = QApplication::desktop()->screenGeometry(); // used to center the mainwindow on desktop

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry((rect.width() - 1280) / 2, (rect.height() - 720) / 2, 1280, 720);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(window));
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);

    mainWindow.show();

    return app.exec();
}
