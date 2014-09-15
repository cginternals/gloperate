
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate/resources/ResourceManager.h>

#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char *argv[])
{
    ContextFormat format;
    format.setVersion(2, 1);

    // Create resource manager
    ResourceManager resourceManager;

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

    // Create main window
    Window window(resourceManager);
    window.setPainter(painter);
    window.setEventHandler(new WindowEventHandler());
    if (window.create(format, "gloperate viewer"))
    {
        // Show window and run application
        window.context()->makeCurrent();
        window.context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);

        globjects::info() << std::endl
            << "OpenGL Version:  " << window.context()->version() << std::endl
            << "OpenGL Vendor:   " << window.context()->vendor() << std::endl
            << "OpenGL Renderer: " << window.context()->renderer() << std::endl;

        window.context()->doneCurrent();

        window.show();
        return MainLoop::run();
    } 
    // Error initializing the window
    return 1;
    }
