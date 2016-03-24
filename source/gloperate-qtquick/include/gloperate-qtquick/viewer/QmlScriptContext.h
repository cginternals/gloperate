
#pragma once


#include <vector>

#include <reflectionzeug/variant/Variant.h>

#include <scriptzeug/backend/AbstractScriptContext.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class QmlEngine;
class QmlObjectWrapper;


/**
*  @brief
*    Scripting backend for use in conjunction with Qt quick
*/
class GLOPERATE_QTQUICK_API QmlScriptContext : public scriptzeug::AbstractScriptContext
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] engine
    *    Qml engine (must NOT be null!)
    */
    QmlScriptContext(QmlEngine * engine);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QmlScriptContext();

    // Virtual AbstractScriptContext functions
    virtual void initialize(scriptzeug::ScriptContext * scriptContext) override;
    virtual void registerObject(reflectionzeug::PropertyGroup * obj) override;
    virtual void unregisterObject(reflectionzeug::PropertyGroup * obj) override;
    virtual reflectionzeug::Variant evaluate(const std::string & code) override;


protected:
    QmlEngine                      * m_engine;         ///< Qml engine with gloperate integration
    std::vector<QmlObjectWrapper*>   m_wrappedObjects; ///< List of wrapped objects owned by the script context
};


} // namespace gloperate_qtquick
