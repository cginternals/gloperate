
#pragma once


#include <QQmlEngine>
#include <QJSValue>
#include <QString>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate {
    class ViewerContext;
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
class GLOPERATE_QTQUICK_API QmlEngine : public QQmlEngine
{
Q_OBJECT
Q_PROPERTY(QJSValue global READ global WRITE setGlobal)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    QmlEngine(gloperate::ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QmlEngine();

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context (cannot be null)
    */
    gloperate::ViewerContext * viewerContext() const;

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
    Q_INVOKABLE QJSValue execute(const QString & code);

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
    reflectionzeug::Variant fromScriptValue(const QJSValue & value);

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
    QJSValue toScriptValue(const reflectionzeug::Variant & var);


protected:
    // Property accessors
    const QJSValue & global() const;
    void setGlobal(const QJSValue & obj);


protected:
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context (must NOT be null)
    QJSValue                   m_global;        ///< Object 'global', can be used to store global values
};


} // namespace gloperate_qtquick
