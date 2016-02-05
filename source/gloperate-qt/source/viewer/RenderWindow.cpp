
#include "gloperate-qt/viewer/RenderWindow.h"


namespace gloperate_qt
{


RenderWindow::RenderWindow()
: OpenGLWindow()
{
}

RenderWindow::RenderWindow(const QSurfaceFormat & format)
: OpenGLWindow(format)
{
}

RenderWindow::~RenderWindow()
{
}


} // namespace gloperate-qt
