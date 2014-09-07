#include <globjects/DebugMessage.h>
#include <gloperate-glfw/ContextFormat.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>


using namespace gloperate_glfw;


int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

//  gloperate::Painter * painter = new SimpleTexture();
    gloperate::Painter * painter = new RotatingQuad();

    WindowEventHandler * eventHandler = new WindowEventHandler();
    eventHandler->setPainter(painter);

    Window window;
    window.setEventHandler(eventHandler);

    if (window.create(format, "Simple Texture Example"))
    {
        window.context()->setSwapInterval(Context::VerticalSyncronization);

        window.show();

        return MainLoop::run();
    }
    else
    {
        return 1;
    }
}
