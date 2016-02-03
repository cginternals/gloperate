
#include <gloperate/viewer/DemoRenderer.h>

#include <iostream>


namespace gloperate
{


DemoRenderer::DemoRenderer()
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

void DemoRenderer::onResize(int width, int height)
{
    std::cout << "onResize(" << width << ", " << height << ")" << std::endl;
}

void DemoRenderer::onRender()
{
    std::cout << "onRender()" << std::endl;
}

void DemoRenderer::onKeyPressed(int key)
{
    std::cout << "onKeyPressed(" << key << ")" << std::endl;
}

void DemoRenderer::onKeyReleased(int key)
{
    std::cout << "onKeyReleased(" << key << ")" << std::endl;
}

void DemoRenderer::onMouseMoved(int x, int y)
{
    std::cout << "onMouseMoved(" << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onMousePressed(int button, int x, int y)
{
    std::cout << "onMousePressed(" << button << ", " << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onMouseReleased(int button, int x, int y)
{
    std::cout << "onMouseReleased(" << button << ", " << x << ", " << y << ")" << std::endl;
}

void DemoRenderer::onTimer(int id)
{
    std::cout << "onTimer(" << id << ")" << std::endl;
}


} // namespace gloperate
