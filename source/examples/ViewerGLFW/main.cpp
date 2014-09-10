#include <iostream>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate-glfw/ContextFormat.h>
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
    format.setVersion(3, 0);

    // Initialize plugin manager
    PluginManager::init(argc > 0 ? argv[0] : "");

    // Load example plugins
    PluginManager pluginManager;
    IF_NDEBUG(pluginManager.loadPlugin("basic-examples");)
    IF_DEBUG(pluginManager.loadPlugin("basic-examplesd");)
    for (Plugin * plugin : pluginManager.plugins()) {
        std::cout << "Plugin '" << plugin->name() << "' (" << plugin->type() << ")\n";
        std::cout << "  by " << plugin->vendor() << "\n";
        std::cout << "  " << plugin->description() << "\n";
        std::cout << "\n";
    }

    // Choose a painter
    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin("RotatingQuad");
    if (plugin) {
        painter = plugin->createPainter();
    } else {
//      painter = new SimpleTexture();
        painter = new RotatingQuad();
    }

    // Create event handler
    WindowEventHandler * eventHandler = new WindowEventHandler();
    eventHandler->setPainter(painter);

    // Create window
    Window window;
    window.setEventHandler(eventHandler);
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
