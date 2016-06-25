
#include <globjects/base/baselogging.h>

#include <gloperate/base/environment.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-glfw/viewer/Application.h>
#include <gloperate-glfw/viewer/RenderWindow.h>
#include <gloperate-glfw/viewer/GLContext.h>

#include <gloperate-ffmpeg/FFMPEGVideoExporter.h>


using namespace gloperate;
using namespace gloperate_glfw;
using namespace gloperate_ffmpeg;


int main(int argc, char * argv[])
{
    // Create gloperate environment
    Environment environment;

    // Initialize GLFW
    Application::init();
    Application app(&environment, argc, argv);

    // Create render stage
    DemoStage * renderStage = new DemoStage(&environment);

    environment.scriptEnvironment()->setupScripting();

    // Create window
    RenderWindow window(&environment);
    window.setRenderStage(renderStage);
    window.setTitle("gloperate viewer");
    window.setSize(1600, 900);
    if (!window.create())
    {
        return 1;
    }

    // Print context info
    window.context()->use();
    globjects::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    window.context()->release();

    FFMPEGVideoExporter videoExporter("gloperate-videotool-example_out.avi", window.canvas(), 30, 5, 1600, 900);
    videoExporter.createVideo([] (int x, int y) { globjects::debug() << "Progress: " << x*100/y <<"%"; });

    return 0;
}
