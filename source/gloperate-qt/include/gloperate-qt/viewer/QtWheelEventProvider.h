
#pragma once


#include <gloperate/ext-includes-begin.h>

#include <QObject>
#include <QWheelEvent>

#include <glm/glm.hpp>

#include <gloperate/ext-includes-end.h>

#include <gloperate/input/AbstractEventProvider.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtWheelEventProvider : public QObject, public gloperate::AbstractEventProvider
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    QtWheelEventProvider();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtWheelEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;


protected:
    static glm::vec2 toDegrees(const glm::ivec2 & eigthsOfADegree);
};


} // namespace gloperate_qt
