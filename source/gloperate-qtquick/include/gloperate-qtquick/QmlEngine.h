
#pragma once


#include <QJSValue>
#include <QString>

#include <cppexpose/signal/Connection.h>
#include <cppexpose/variant/Variant.h>

#include <qmltoolbox/QmlApplicationEngine.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QVariant;

namespace cppexpose
{
    class Object;
}

namespace gloperate
{
    class Environment;
}


namespace gloperate_qtquick
{


class QmlObjectWrapper;


/**
*  @brief
*    Qml engine with gloperate integration
*
*    This is an extended QML engine that adds an integration
*    layer for gloperate. It imports the gloperate namespace
*    for qml and adds an global object named 'global' which
*    can be used by scripting functions to store data globally.
*/
class GLOPERATE_QTQUICK_API QmlEngine : public qmltoolbox::QmlApplicationEngine
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Gloperate environment (must NOT be null!)
    */
    QmlEngine(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QmlEngine();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Gloperate environment (cannot be null)
    */
    const gloperate::Environment * environment() const;

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Gloperate environment (cannot be null)
    */
    gloperate::Environment * environment();

    /**
    *  @brief
    *    Add a global object that is exposed into the scripting environment
    *
    *  @param[in] obj
    *    Global object (must NOT be null)
    */
    void addGlobalObject(cppexpose::Object * obj);

    /**
    *  @brief
    *    Remove a global object that is exposed into the scripting environment
    *
    *  @param[in] obj
    *    Global object (must NOT be null)
    */
    void removeGlobalObject(cppexpose::Object * obj);

    /**
    *  @brief
    *    Execute script
    *
    *  @param[in] code
    *    Script code
    *
    *  @return
    *    Return value of the script
    */
    Q_INVOKABLE QString executeScript(const QString & code);

    /**
    *  @brief
    *    Convert script value to variant
    *
    *  @param[in] value
    *    Script value
    *
    *  @return
    *    Variant value
    */
    cppexpose::Variant fromScriptValue(const QJSValue & value);

    /**
    *  @brief
    *    Convert variant to script value
    *
    *  @param[in] var
    *    Variant value
    *
    *  @return
    *    Script value
    */
    QJSValue toScriptValue(const cppexpose::Variant & var);

    /**
    *  @brief
    *    Convert QVariant to variant
    *
    *  @param[in] value
    *    QVariant
    *
    *  @return
    *    Variant value
    */
    cppexpose::Variant fromQVariant(const QVariant & value);

    /**
    *  @brief
    *    Get path to gloperate qml module
    *
    *  @return
    *    Path to gloperate qml module
    */
    const QString & gloperateModulePath() const;

    /**
    *  @brief
    *    Get existing wrapper for object or create a new one
    *
    *  @param[in] object
    *    Object to be wrapped (must NOT be null)
    *
    *  @return
    *    Object wrapper (never null)
    *
    *  @remarks
    *    The returned object wrapper is owned by the backend
    *    and will be deleted if either the backend or the
    *    wrapped object is destroyed
    */
    QmlObjectWrapper * getOrCreateObjectWrapper(cppexpose::Object * object);


protected:
    gloperate::Environment                                                            * m_environment;      ///< Gloperate environment (must NOT be null)
    QString                                                                             m_gloperateQmlPath; ///< Path to gloperate qml module
    std::map<cppexpose::Object *, std::pair<QmlObjectWrapper *, cppexpose::Connection>> m_objectWrappers;   ///< Global objects
};


} // namespace gloperate_qtquick
