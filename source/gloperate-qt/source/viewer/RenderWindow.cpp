
#include "gloperate-qt/viewer/RenderWindow.h"

#include <gloperate/viewer/DemoRenderer.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext)
: RenderWindow(viewerContext, new gloperate::DemoRenderer(viewerContext))
{
}

RenderWindow::~RenderWindow()
{
    delete m_surface;
}

RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext, gloperate::Surface * surface)
: OpenGLWindow(surface->negotiateContext())
, m_viewerContext(viewerContext)
, m_surface(surface)
{
}

gloperate::ViewerContext * RenderWindow::viewerContext() const
{
    return m_viewerContext;
}

gloperate::Surface * RenderWindow::surface() const
{
    return m_surface;
}


} // namespace gloperate-qt
