
#pragma once


#include <vector>

#include <QObject>
#include <QJSValue>

#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace cppexpose {
    class Object;
}


namespace gloperate_qtquick
{


class QmlEngine;


/**
*  @brief
*    QObject representation of a cppexpose::Object
*
*    This class is used to expose a cppexpose::Object trough
*    a QObject interface into a QML scripting environment.
*/
class GLOPERATE_QTQUICK_API QmlObjectWrapper : public QObject
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] engine
    *    Qml engine
    *  @param[in] obj
    *    Object (must NOT be null!)
    */
    QmlObjectWrapper(QmlEngine * engine, cppexpose::Object * obj);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QmlObjectWrapper();

    /**
    *  @brief
    *    Create wrapper object in the scripting environment
    *
    *  @return
    *    Script value wrapping the object
    */
    QJSValue wrapObject();


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
    QmlEngine                       * m_engine;           ///< Qml engine with gloperate integration
    cppexpose::Object               * m_object;           ///< Wrapped object (must NOT be null)
    QJSValue                          m_obj;              ///< Javascript object representing the cppexpose object
    std::vector<QmlObjectWrapper *>   m_wrappedObjects;   ///< List of wrapped sub-objects
    QJSValue                          m_registerProperty; ///< Helper script for registering properties
    QJSValue                          m_registerFunction; ///< Helper script for registering functions

    // Connections to the wrapped object
    cppexpose::ScopedConnection m_beforeDestroyConnection;
    cppexpose::ScopedConnection m_afterAddConnection;
    cppexpose::ScopedConnection m_beforeRemoveConnection;
};


} // namespace gloperate_qtquick
