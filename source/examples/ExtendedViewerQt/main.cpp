/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <iostream>
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <gloperate-qt/qt-includes-end.h>
#include <reflectionzeug/PropertyGroup.h>
#include <propertyguizeug/PropertyBrowser.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <gloperate-qt/QtTextureStorer.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Create resource manager
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());
    resourceManager.addStorer(new QtTextureStorer());

    // Initialize plugin manager
    PluginManager::init(argc > 0 ? argv[0] : "");

    // Load example plugins
    PluginManager pluginManager;
    pluginManager.scan("examples");

    // Choose a painter
    std::string name = (argc > 1) ? argv[1] : "ExtendedCubeScape";
    std::cout << "Trying to create painter '" << name << "'\n";

    // Create painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin(name);
    if (plugin) {
        painter = plugin->createPainter(resourceManager);
    } else {
        // Error, could not find plugin
        std::cout << "Could not find plugin '" << name << "'\n";
        pluginManager.printPlugins();
        return 1;
    }

    // Create OpenGL window
    QtOpenGLWindow * glWindow = new QtOpenGLWindow(resourceManager);
    if (painter) {
        glWindow->setPainter(painter);
    }

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    // If the painter has properties, display them in a property browser
    reflectionzeug::PropertyGroup * properties = dynamic_cast<reflectionzeug::PropertyGroup *>(painter);
    if (properties) {
        // Create property browser as a dock widget
        QDockWidget * dock = new QDockWidget(&mainWindow);
        dock->setWidget(new propertyguizeug::PropertyBrowser(properties, dock));
        mainWindow.addDockWidget(Qt::LeftDockWidgetArea, dock);
    }

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}
