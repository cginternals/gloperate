
#pragma once


#include <QJSValue>
#include <QString>

#include <cppexpose/variant/Variant.h>

#include <qmltoolbox/QmlApplicationEngine.h>
#include <qmltoolbox/QmlEngine.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QVariant;

namespace gloperate
{
    class Environment;
}


namespace gloperate_qtquick
{


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
    Q_PROPERTY(QJSValue global    READ global    WRITE setGlobal)
    Q_PROPERTY(QJSValue gloperate READ gloperate WRITE setGloperate)


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
    *    Get object 'global'
    *
    *  @return
    *    Object
    */
    const QJSValue & global() const;

    /**
    *  @brief
    *    Get object 'global'
    *
    *  @return
    *    Object
    */
    QJSValue & global();

    /**
    *  @brief
    *    Set object 'global'
    *
    *  @param[in] obj
    *    Object
    */
    void setGlobal(const QJSValue & obj);

    /**
    *  @brief
    *    Get object 'gloperate'
    *
    *  @return
    *    Object
    */
    const QJSValue & gloperate() const;

    /**
    *  @brief
    *    Get object 'gloperate'
    *
    *  @return
    *    Object
    */
    QJSValue & gloperate();

    /**
    *  @brief
    *    Set object 'gloperate'
    *
    *  @param[in] obj
    *    Object
    */
    void setGloperate(const QJSValue & obj);

    /**
    *  @brief
    *    Get path to gloperate qml module
    *
    *  @return
    *    Path to gloperate qml module
    */
    const QString & gloperateModulePath() const;


protected:
    gloperate::Environment * m_environment;      ///< Gloperate environment (must NOT be null)
    QJSValue                 m_global;           ///< Object 'global', can be used to store global values
    QJSValue                 m_gloperate;        ///< Object 'gloperate', contains exposed API functions from gloperate
    QString                  m_gloperateQmlPath; ///< Path to gloperate qml module
};


} // namespace gloperate_qtquick
