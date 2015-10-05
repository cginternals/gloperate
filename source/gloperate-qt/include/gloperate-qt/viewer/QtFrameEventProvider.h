#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate-qt/viewer/AbstractQtEventProvider.h>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <gloperate/ext-includes-end.h>

#include <gloperate/input/FrameEvent.h>

namespace gloperate_qt {

class GLOPERATE_QT_API QtFrameEventProvider: public QObject, public AbstractQtEventProvider
{
public:
    using Clock = gloperate::FrameEvent::Clock;

public:
    virtual bool eventFilter(QObject * obj, QEvent * event) override;

protected:
    Clock::time_point m_lastFrameTick;
};

} /* namespace gloperate_qt */
