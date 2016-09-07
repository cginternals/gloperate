
#pragma once


#include <gloperate/ext-includes-begin.h>

#include <QObject>
#include <QWheelEvent>

#include <glm/glm.hpp>

#include <gloperate-qt/gloperate-qt_api.h>
#include <gloperate-qt/viewer/AbstractQtEventProvider.h>


namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtWheelEventProvider : public QObject, public AbstractQtEventProvider
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    explicit QtWheelEventProvider(QObject * parent = nullptr);

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
