
#include <gloperate-qt/viewer/AbstractQtMapping.h>

#include <gloperate/ext-includes-begin.h>
#include <QOpenGLContext>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate;


namespace gloperate_qt
{


AbstractQtMapping::AbstractQtMapping(QtOpenGLWindow * window)
    :   AbstractMapping()
    ,   m_window(window)
{
}

AbstractQtMapping::~AbstractQtMapping()
{
}

void AbstractQtMapping::processEvent(AbstractEvent * event)
{
    m_window->context()->makeCurrent(m_window);
    mapEvent(event);
    m_window->context()->doneCurrent();
}


} // namespace gloperate_qt
