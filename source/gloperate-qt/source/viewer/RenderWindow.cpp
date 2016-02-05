
#include "gloperate-qt/viewer/RenderWindow.h"

#include <gloperate/viewer/DemoRenderer.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext)
: m_surface(new gloperate::DemoRenderer(viewerContext))
{
}

RenderWindow::~RenderWindow()
{
    delete m_surface;
}

gloperate::Surface * RenderWindow::surface() const
{
    return m_surface;
}


} // namespace gloperate-qt
