
#pragma once

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <QKeyEvent>
#include <gloperate/ext-includes-end.h>

#include <gloperate/input/input.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/AbstractEventProvider.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtKeyEventProvider : public QObject, public gloperate::AbstractEventProvider
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    QtKeyEventProvider();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtKeyEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;
};


} // namespace gloperate_qt
