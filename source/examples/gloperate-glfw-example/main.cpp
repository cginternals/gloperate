
#include <cppassist/logging/logging.h>

#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-glfw/viewer/Application.h>
#include <gloperate-glfw/viewer/RenderWindow.h>
#include <gloperate-glfw/viewer/GLContext.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char * argv[])
{
    // Create viewer context
    ViewerContext viewerContext;

    // Configure and load plugins
    viewerContext.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    viewerContext.componentManager()->scanPlugins("loaders");
    viewerContext.componentManager()->scanPlugins("stages");

    // Initialize GLFW
    Application::init();
    Application app(&viewerContext, argc, argv);

    // Create render stage
    DemoStage * renderStage = new DemoStage(&viewerContext);

    // Create render window
    RenderWindow window(&viewerContext);
    window.setRenderStage(renderStage);
    window.setTitle("gloperate viewer");
    window.setSize(1280, 720);
    if (!window.create())
    {
        return 1;
    }

    // Initialize context, print context info
    window.context()->use();
//  window.context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
    cppassist::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    window.context()->release();

    // Display window
    window.show();

    // Run main loop
    return app.run();
}
