
#include "gloperate-glheadless/base/RenderSurface.h"

#include <globjects/Framebuffer.h>

#include <gloperate/base/Canvas.h>

#include <gloperate-glheadless/base/GLContext.h>


namespace gloperate_headless
{


RenderSurface::RenderSurface(gloperate::Environment * environment)
: m_environment(environment)
, m_canvas(new gloperate::Canvas(environment))
{
}


RenderSurface::~RenderSurface()
{
}


gloperate::Environment * RenderSurface::environment() const
{
    return m_environment;
}


gloperate::Stage * RenderSurface::renderStage() const
{
    return m_canvas->renderStage();
}


void RenderSurface::setRenderStage(gloperate::Stage * stage)
{
    m_canvas->setRenderStage(stage);
}


void RenderSurface::onContextInit()
{
    m_canvas->setOpenGLContext(m_context.get());
}


void RenderSurface::onContextDeinit()
{
    m_canvas->setOpenGLContext(nullptr);
}


void RenderSurface::onResize(int width, int height)
{
    const auto viewport = glm::ivec4(0, 0, width, height);
    m_canvas->onViewport(viewport, viewport);
}


void RenderSurface::onPaint()
{
    m_canvas->onRender();
}


} // namespace gloperate_headless
