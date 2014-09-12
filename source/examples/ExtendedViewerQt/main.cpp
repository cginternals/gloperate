/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <extended-examples/ExtendedCubeScape/ExtendedCubeScape.h>

#include <reflectionzeug/PropertyGroup.h>

#include <propertyguizeug/PropertyBrowser.h>

#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>
#include <gloperate/base/ChronoTimer.h>


using namespace gloperate;
using namespace gloperate_qt;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Initialize plugin manager
    PluginManager::init(argc > 0 ? argv[0] : "");

    // Create plugin manager
    PluginManager pluginManager;
    pluginManager.scan("examples");
    pluginManager.printPlugins();

    // Create resource manager
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());

    // Choose a painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin("ExtendedCubeScape");
    if (plugin) {
        painter = plugin->createPainter();
    } else {
//      painter = new SimpleTexture();
        painter = new ExtendedCubeScape(&resourceManager);
    }

    // Create OpenGL window
    QtOpenGLWindow * glWindow = new QtOpenGLWindow();
    if (painter) {
        glWindow->setPainter(painter);
    }

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    reflectionzeug::PropertyGroup * properties = dynamic_cast<reflectionzeug::PropertyGroup *>(painter);

    if (properties != nullptr)
    {
        QDockWidget * dock = new QDockWidget(&mainWindow);

        dock->setWidget(new propertyguizeug::PropertyBrowser(properties, dock));

        mainWindow.addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}

