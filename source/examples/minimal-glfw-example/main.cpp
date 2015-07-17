
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/PainterPlugin.h>

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

    ResourceManager resourceManager;

    // Setup plugin manager
    PluginManager pluginManager;
    pluginManager.addPath(app.applicationPath());
#ifdef NDEBUG
    pluginManager.addPath("plugins");
#else
    pluginManager.addPath("plugins/debug");
#endif
    pluginManager.scan("painters");

    // Choose a painter
    std::unique_ptr<gloperate::Painter> painter(nullptr);

    std::string name = (argc > 1) ? argv[1] : "CubeScape";

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

    Window::init();

    Window window(resourceManager);
    window.setPainter(painter.get());
    window.setEventHandler(new WindowEventHandler());

    ContextFormat format;
    format.setVersion(3, 2);

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
