
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

void DemoRenderer::onResize(const glm::ivec2 & deviceSize, const glm::ivec2 & virtualSize)
{
    std::cout << "onResize(" << deviceSize.x << ", " << deviceSize.y << ", " << virtualSize.x << ", " << virtualSize.y << ")" << std::endl;
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

void DemoRenderer::onMouseMove(const glm::ivec2 & pos)
{
    std::cout << "onMouseMoved(" << pos.x << ", " << pos.y << ")" << std::endl;
}

void DemoRenderer::onMousePress(int button, const glm::ivec2 & pos)
{
    std::cout << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")" << std::endl;
}

void DemoRenderer::onMouseRelease(int button, const glm::ivec2 & pos)
{
    std::cout << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")" << std::endl;
}

void DemoRenderer::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    std::cout << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")" << std::endl;
}

void DemoRenderer::onTimer(int id)
{
    std::cout << "onTimer(" << id << ")" << std::endl;
}


} // namespace gloperate
