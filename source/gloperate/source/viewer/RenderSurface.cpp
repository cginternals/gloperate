
#include <gloperate/viewer/RenderSurface.h>

#include <globjects/base/baselogging.h>


namespace gloperate
{


RenderSurface::RenderSurface(ViewerContext * viewerContext)
: Surface(viewerContext)
{
}

RenderSurface::~RenderSurface()
{
}

void RenderSurface::onContextInit()
{
    globjects::info() << "onContextInit()";
}

void RenderSurface::onContextDeinit()
{
    globjects::info() << "onContextDeinit()";
}

void RenderSurface::onIdle()
{
//  globjects::info() << "onIdle()";
}

void RenderSurface::onResize(const glm::ivec2 & deviceSize, const glm::ivec2 & virtualSize)
{
    globjects::info() << "onResize(" << deviceSize.x << ", " << deviceSize.y << ", " << virtualSize.x << ", " << virtualSize.y << ")";
}

void RenderSurface::onRender()
{
    globjects::info() << "onRender()";
}

void RenderSurface::onKeyPress(int key)
{
    globjects::info() << "onKeyPressed(" << key << ")";
}

void RenderSurface::onKeyRelease(int key)
{
    globjects::info() << "onKeyReleased(" << key << ")";
}

void RenderSurface::onMouseMove(const glm::ivec2 & pos)
{
    globjects::info() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMousePress(int button, const glm::ivec2 & pos)
{
    globjects::info() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseRelease(int button, const glm::ivec2 & pos)
{
    globjects::info() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    globjects::info() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onTimer(int id)
{
    globjects::info() << "onTimer(" << id << ")";
}


} // namespace gloperate
