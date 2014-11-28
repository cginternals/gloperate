#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate/navigation/AbstractMapping.h>

namespace gloperate {
    class AbstractEvent;
}

using namespace gloperate;

namespace gloperate_qt
{
class QtOpenGLWindow;

class GLOPERATE_QT_API AbstractQtMapping : public AbstractMapping
{
public:
    AbstractQtMapping(QtOpenGLWindow * window);
    virtual ~AbstractQtMapping();

    virtual void processEvent(AbstractEvent * event);

protected:
    QtOpenGLWindow * m_window;
};

} // namespace gloperate_qt
