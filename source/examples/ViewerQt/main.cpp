#include <iostream>
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QMainWindow>
#include <QScopedPointer>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>
#include <gloperate/ChronoTimer.h>
#include <gloperate-qt/QtOpenGLWindow.h>
#include <gloperate-qt/QtTextureLoader.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>

#include <gloperate-qt/TimePropagator.h>


using namespace gloperate;
using namespace gloperate_qt;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize plugin manager
    PluginManager::init(argc > 0 ? argv[0] : "");

    // Load example plugins
    PluginManager pluginManager;
    pluginManager.scan("examples");
    for (Plugin * plugin : pluginManager.plugins()) {
        std::cout << "Plugin '" << plugin->name() << "' (" << plugin->type() << ")\n";
        std::cout << "  version " << plugin->version() << "\n";
        std::cout << "  by " << plugin->vendor() << "\n";
        std::cout << "  " << plugin->description() << "\n";
        std::cout << "\n";
    }

    // Create resource manager
    ResourceManager resourceManager;
    resourceManager.addLoader(new QtTextureLoader());

    // Choose a painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin("CubeScape");
    if (plugin) {
        painter = plugin->createPainter();
    } else {
//      painter = new SimpleTexture();
        painter = new RotatingQuad(&resourceManager);
    }

    QScopedPointer<TimePropagator> timePropagator(nullptr);

    // Create OpenGL window
    QtOpenGLWindow * glWindow = new QtOpenGLWindow();
    if (painter) {
        // Set painter to window
        glWindow->setPainter(painter);

        // Initialize virtual time propagator
        if (painter->supports<gloperate::AbstractVirtualTimeCapability>()) {
            timePropagator.reset(new TimePropagator(glWindow, painter->getCapability<gloperate::AbstractVirtualTimeCapability>()));
        }
    }

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 600);
    mainWindow.setCentralWidget(QWidget::createWindowContainer(glWindow));

    // Show window
    mainWindow.show();

    // Run application
    return app.exec();
}
