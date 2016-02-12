
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/Surface.h>

#include <gloperate-glfw/Application.h>
#include <gloperate-glfw/RenderWindow.h>
#include <gloperate-glfw/GLContext.h>
#include <gloperate-glfw/GLContextFactory.h>


using namespace gloperate;
using namespace gloperate_glfw;


int main(int argc, char * argv[])
{
    // Initialize GLFW
    Application::init();
    Application app(argc, argv);

    // Create context factory
    GLContextFactory contextFactory;

    // Create viewer context
    ViewerContext viewerContext(contextFactory);

    // Create render window
    RenderWindow window(&viewerContext);
//    window.setTitle("gloperate viewer");
//    window.setSize(1280, 720);
//    window.setFormat(window.surface()->negotiateContext());
    if (!window.create())
    {
        return 1;
    }

    // Initialize context, print context info
    window.context()->use();
//  window.context()->setSwapInterval(Context::SwapInterval::VerticalSyncronization);
    globjects::info() << std::endl
        << "OpenGL Version:  " << GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << GLContextUtils::renderer() << std::endl;
    window.context()->release();

    // Display window
    window.show();

    // Run main loop
    return app.run();
}
