
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate/resources/ResourceManager.h>

#include <gloperate-glfw/Application.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char * argv[])
{
    Application app(argc, argv);

    ContextFormat format;
    format.setVersion(3, 2);

    ResourceManager resourceManager;

    PluginManager::init();

    std::cout << "moep" << std::endl;

    PluginManager pluginManager;
    pluginManager.scan("examples");

    // Choose a painter
    std::string name = (argc > 1) ? argv[1] : "CubeScape";

    gloperate::Painter * painter = nullptr;
    Plugin * plugin = pluginManager.plugin(name);

    if (!plugin) 
    {
        globjects::fatal() << "Plugin '" << name << "' not found. Listing plugins found:";
        pluginManager.printPlugins();

        return 1;
    }

    painter = plugin->createPainter(resourceManager);

    Window::init();

    Window window(resourceManager);
    window.setPainter(painter);
    window.setEventHandler(new WindowEventHandler());

    if (!window.create(format, "gloperate viewer"))
        return 1;

    window.context()->makeCurrent();
    window.context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);

    globjects::info() << std::endl
        << "OpenGL Version:  " << window.context()->version() << std::endl
        << "OpenGL Vendor:   " << window.context()->vendor() << std::endl
        << "OpenGL Renderer: " << window.context()->renderer() << std::endl;

    window.context()->doneCurrent();
    window.show();

    return app.exec();
}
