#include <iostream>
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <gloperate-qt/QtTextureStorer.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char *argv[])
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
    std::string name = (argc > 1) ? argv[1] : "CubeScape";
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
        // Set painter to window
        glWindow->setPainter(painter);
    }

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));
    mainWindow.centralWidget()->setFocusPolicy(Qt::StrongFocus);

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}
