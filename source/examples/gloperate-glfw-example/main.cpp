
#include <cppassist/logging/logging.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-glfw/Application.h>
#include <gloperate-glfw/RenderWindow.h>
#include <gloperate-glfw/GLContext.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char * argv[])
{
    // Create gloperate environment
    Environment environment;

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        gloperate::pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins("loaders");
    environment.componentManager()->scanPlugins("stages");

    // Initialize GLFW
    Application::init();
    Application app(&environment, argc, argv);

    // Create render stage
    DemoStage * renderStage = new DemoStage(&environment);

    // Create render window
    RenderWindow window(&environment);
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
