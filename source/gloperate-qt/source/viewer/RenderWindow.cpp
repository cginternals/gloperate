
#include "gloperate-qt/viewer/RenderWindow.h"


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::ViewerContext *)
: OpenGLWindow()
{
}

RenderWindow::RenderWindow(gloperate::ViewerContext *, const QSurfaceFormat & format)
: OpenGLWindow(format)
{
}

RenderWindow::~RenderWindow()
{
}


} // namespace gloperate-qt
