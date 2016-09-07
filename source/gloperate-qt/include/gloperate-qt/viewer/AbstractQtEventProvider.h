#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate/input/AbstractEventProvider.h>

class QObject;

namespace gloperate_qt {

class GLOPERATE_QT_API AbstractQtEventProvider: public gloperate::AbstractEventProvider
{
public:
    /**
     * If obj is a QtOpenGLWindow, calls makeCurrent() on it before passing event on.
     */
    virtual void passEventWithContext(QObject * obj, gloperate::AbstractEvent * event);
};

} /* namespace gloperate_qt */
