
#pragma once


#include <QObject>
#include <QJSValue>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace reflectionzeug {
    class Object;
}


namespace gloperate_qtquick
{


class QmlEngine;


/**
*  @brief
*    QObject representation of a libzeug::Object
*
*    This class is used to expose a libzeug::Object trough
*    a QObject interface into a QML scripting environment.
*/
class GLOPERATE_QTQUICK_API ObjectWrapper : public QObject
{
Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] engine
    *    Qml engine
    *  @param[in] object
    *    Wrapper object (must NOT be nullptr)
    */
    ObjectWrapper(QmlEngine * engine, reflectionzeug::Object * object);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ObjectWrapper();


protected:
    /**
    *  @brief
    *    Get value of property
    *
    *  @param[in] name
    *    Property name
    *
    *  @return
    *    Property value
    */
    Q_INVOKABLE QJSValue getProp(const QString & name);

    /**
    *  @brief
    *    Set value of property
    *
    *  @param[in] name
    *    Property name
    *  @param[in] value
    *    Property value
    */
    Q_INVOKABLE void setProp(const QString & name, const QJSValue & value);

    /**
    *  @brief
    *    Call function of object
    *
    *  @param[in] name
    *    Function name
    *  @param[in] args
    *    Arguments as array
    */
    Q_INVOKABLE QJSValue callFunc(const QString & name, const QJSValue & args);


protected:
    QmlEngine              * m_engine; ///< Qml engine with gloperate integration
    reflectionzeug::Object * m_object; ///< Wrapped object
};


} // namespace gloperate_qtquick
