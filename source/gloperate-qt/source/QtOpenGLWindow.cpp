#include "gloperate-qt/QtOpenGLWindow.h"
#include <gloperate-qt/qt-includes-begin.h>
#include <QResizeEvent>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate/Viewport.h>


using namespace gloperate;
namespace gloperate_qt
{


QtOpenGLWindow::QtOpenGLWindow()
: QtOpenGLWindowBase()
{
}

QtOpenGLWindow::QtOpenGLWindow(const QSurfaceFormat & format)
: QtOpenGLWindowBase(format)
{
}

QtOpenGLWindow::~QtOpenGLWindow()
{
}

Painter * QtOpenGLWindow::painter() const
{
    return m_painter;
}

void QtOpenGLWindow::setPainter(Painter * painter)
{
    m_painter = painter;
}

void QtOpenGLWindow::onInitialize()
{
    if (m_painter) {
        m_painter->initialize();
    }
}

void QtOpenGLWindow::onResize(QResizeEvent * event)
{
    if (m_painter) {
        m_painter->resize(Viewport(0, 0, event->size().width(), event->size().height()));
    }
}

void QtOpenGLWindow::onPaint()
{
    if (m_painter) {
        m_painter->paint();
    }
}


} // namespace gloperate-qt
