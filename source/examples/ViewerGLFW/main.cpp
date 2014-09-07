#include <globjects/DebugMessage.h>
#include <gloperate-glfw/ContextFormat.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandler.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>


using namespace gloperate_glfw;


int main(int /*argc*/, char* /*argv*/[])
{
    ContextFormat format;
    format.setVersion(3, 0);

    WindowEventHandler * eventHandler = new WindowEventHandler();
    eventHandler->setPainter(new SimpleTexture());

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
