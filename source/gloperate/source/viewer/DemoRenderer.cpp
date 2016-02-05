
#include <gloperate/viewer/DemoRenderer.h>

#include <iostream>


namespace gloperate
{


DemoRenderer::DemoRenderer(ViewerContext * viewerContext)
: Surface(viewerContext)
{
}

DemoRenderer::~DemoRenderer()
{
}

void DemoRenderer::onContextInit()
{
    std::cout << "onContextInit()" << std::endl;
}

void DemoRenderer::onContextDeinit()
{
    std::cout << "onContextDeinit()" << std::endl;
}

void DemoRenderer::onIdle()
{
//  std::cout << "onIdle()" << std::endl;
}

void DemoRenderer::onResize(int deviceWidth, int deviceHeight, int virtualWidth, int virtualHeight)
{
    std::cout << "onResize(" << deviceWidth << ", " << deviceHeight << ", " << virtualWidth << ", " << virtualHeight << ")" << std::endl;
}

void DemoRenderer::onRender()
{
    std::cout << "onRender()" << std::endl;
}

void DemoRenderer::onKeyPress(int key)
{
    std::cout << "onKeyPressed(" << key << ")" << std::endl;
}

void DemoRenderer::onKeyRelease(int key)
{
    std::cout << "onKeyReleased(" << key << ")" << std::endl;
}

void DemoRenderer::onMouseMove(int x, int y)
{
    std::cout << "onMouseMoved(" << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onMousePress(int button, int x, int y)
{
    std::cout << "onMousePressed(" << button << ", " << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onMouseRelease(int button, int x, int y)
{
    std::cout << "onMouseReleased(" << button << ", " << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onMouseWheel(float dx, float dy, int x, int y)
{
    std::cout << "onMouseWheel(" << dx << ", " << dy << ", " << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onTimer(int id)
{
    std::cout << "onTimer(" << id << ")" << std::endl;
}


} // namespace gloperate
