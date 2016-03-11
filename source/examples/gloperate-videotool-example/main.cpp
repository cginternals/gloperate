
#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/stages/demos/DemoStage.h>

#include <gloperate-glfw/viewer/Application.h>
#include <gloperate-glfw/viewer/RenderWindow.h>
#include <gloperate-glfw/viewer/GLContext.h>

#include <gloperate-ffmpeg/VideoTool.h>


using namespace gloperate;
using namespace gloperate_glfw;
using namespace gloperate_ffmpeg;


int main(int argc, char * argv[])
{
    // Create viewer context
    ViewerContext viewerContext;

    // Initialize GLFW
    Application::init();
    Application app(&viewerContext, argc, argv);

    // Create render stage
    DemoStage * renderStage = new DemoStage(&viewerContext);

    // Create window
    RenderWindow window(&viewerContext, renderStage);
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

    VideoTool videoTool("gloperate-videotool-example_out.avi", window.renderSurface(), 30, 5, 1600, 900);
    videoTool.createVideo([] (int x, int y) { globjects::debug() << "Progress: " << x*100/y <<"%"; });

    return 0;
}
