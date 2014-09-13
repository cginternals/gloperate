#include <iostream>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate-glfw/ContextFormat.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char *argv[])
{
    ContextFormat format;
    format.setVersion(3, 0);

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
        painter = plugin->createPainter();
    } else {
        // Error, could not find plugin
        std::cout << "Could not find plugin '" << name << "'\n";
        pluginManager.printPlugins();
        return 1;
    }

    // Create main window
    Window window;
    window.setPainter(painter);
    window.setEventHandler(new WindowEventHandler());
    if (window.create(format, "gloperate viewer")) {
        // Show window and run application
        window.context()->setSwapInterval(Context::VerticalSyncronization);
        window.show();
        return MainLoop::run();
    } else {
        // Error initializing the window
        return 1;
    }
}
