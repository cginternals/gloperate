
#include <iostream>

#include <cppassist/logging/logging.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-glheadless/base/RenderSurface.h>
#include <gloperate-glheadless/base/GLContext.h>


using namespace gloperate;
using namespace gloperate_headless;


int main(int argc, char * argv[])
{
    // Create gloperate environment
    Environment environment;
    environment.setupScripting();

    // Configure and load plugins
    environment.componentManager()->addPluginPath(
        pluginPath(), cppexpose::PluginPathType::Internal
    );
    environment.componentManager()->scanPlugins("loaders");
    environment.componentManager()->scanPlugins("stages");
    environment.componentManager()->scanPlugins("exporter");

    // Create render stage
    DemoStage * renderStage = new DemoStage(&environment);

    RenderSurface surface(&environment);
    surface.setRenderStage(renderStage);
    surface.createContext();

    // Initialize context, print context info
    surface.context()->use();
    cppassist::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Profile:  " << GLContextUtils::profile() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    surface.context()->release();

    // Set up exit handler
    auto shouldExit = false;
    auto exitCode = EXIT_SUCCESS;
    environment.exitApplication.connect([&shouldExit, &exitCode] (int code)
    {
        shouldExit = true;
        exitCode = code;
    });

    // Enter script loop
    std::cout << "Enter command:" << std::endl;
    while (!shouldExit)
    {
        std::cout << "> " << std::flush;

        std::string command;
        std::getline(std::cin, command);

        const auto result = environment.executeScript(command);
        std::cout << result.value<std::string>();

        surface.updateGL();
    }

    return exitCode;
}
