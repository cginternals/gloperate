
#pragma once


#include <reflectionzeug/variant/Variant.h>

#include <scriptzeug/backend/AbstractScriptContext.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class QmlEngine;


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
    *    Qml engine
    */
    ScriptContext(scriptzeug::ScriptContext * scriptContext, QmlEngine * engine);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptContext();

    // Virtual AbstractScriptContext functions
    virtual void registerObject(reflectionzeug::PropertyGroup * obj) override;
    virtual void unregisterObject(reflectionzeug::PropertyGroup * obj) override;
    virtual reflectionzeug::Variant evaluate(const std::string & code) override;


protected:
    QmlEngine * m_engine; ///< Qml engine with gloperate integration
};


} // namespace gloperate_qtquick
