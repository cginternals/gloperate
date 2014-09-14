
#include <iostream>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>

#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char *argv[])
{
    ContextFormat format;
    format.setVersion(2, 1);

    // Initialize plugin manager
    PluginManager::init(argc > 0 ? argv[0] : "");

    // Load example plugins
    PluginManager pluginManager;
    pluginManager.scan("examples");
    for (Plugin * plugin : pluginManager.plugins()) {
        std::cout << "Plugin '" << plugin->name() << "' (" << plugin->type() << ")\n";
        std::cout << "  by " << plugin->vendor() << "\n";
        std::cout << "  " << plugin->description() << "\n";
        std::cout << "\n";
    }

    // Choose a painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin("RotatingQuad");
    if (plugin) 
    {
        painter = plugin->createPainter();
    }
    else 
    {
//      painter = new SimpleTexture();
        painter = new RotatingQuad();
    }

    // Create event handler
    WindowEventHandler * eventHandler = new WindowEventHandler();

    Window::init();

    // Create window
    Window window;
    window.setPainter(painter);
    window.setEventHandler(eventHandler);

    if (window.create(format, "gloperate viewer")) 
    {
        window.context()->makeCurrent();
        window.context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);

        globjects::info() << std::endl
            << "OpenGL Version:  " << window.context()->version()  << std::endl
            << "OpenGL Vendor:   " << window.context()->vendor()   << std::endl
            << "OpenGL Renderer: " << window.context()->renderer() << std::endl;

        window.context()->doneCurrent();

        window.show();
        return MainLoop::run();
    } 
    // Error initializing the window
    return 1;
}
