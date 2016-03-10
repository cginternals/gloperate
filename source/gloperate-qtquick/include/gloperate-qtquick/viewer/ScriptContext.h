
#pragma once


#include <reflectionzeug/variant/Variant.h>

#include <scriptzeug/backend/AbstractScriptContext.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QJSEngine;
class QJSValue;


namespace gloperate_qtquick
{


/**
*  @brief
*    Scripting backend for use in conjunction with Qt quick
*/
class GLOPERATE_QTQUICK_API ScriptContext : public scriptzeug::AbstractScriptContext
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] scriptContext
    *    Script context that owns the backend (must NOT be null)
    *  @param[in] engine
    *    Qt javascript engine used as backend
    */
    ScriptContext(scriptzeug::ScriptContext * scriptContext, QJSEngine * engine);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptContext();

    // Virtual AbstractScriptContext functions
    virtual void registerObject(reflectionzeug::PropertyGroup * obj) override;
    virtual void unregisterObject(reflectionzeug::PropertyGroup * obj) override;
    virtual reflectionzeug::Variant evaluate(const std::string & code) override;

    // Helper functions
    reflectionzeug::Variant fromScriptValue(const QJSValue & value);
    QJSValue toScriptValue(const reflectionzeug::Variant & var);


protected:
    QJSEngine * m_engine; ///< Qt javascript engine used as backend
};


} // namespace gloperate_qtquick
