#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate/navigation/AbstractMapping.h>

namespace gloperate {
    class AbstractEvent;
}

namespace gloperate_qt
{
class QtOpenGLWindow;

class GLOPERATE_QT_API AbstractQtMapping : public gloperate::AbstractMapping
{
public:
    AbstractQtMapping(QtOpenGLWindow * window);
    virtual ~AbstractQtMapping();

    virtual void processEvent(gloperate::AbstractEvent * event);

protected:
    QtOpenGLWindow * m_window;
};

} // namespace gloperate_qt
