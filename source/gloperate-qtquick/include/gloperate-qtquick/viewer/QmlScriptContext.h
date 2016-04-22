
#pragma once


#include <vector>

#include <cppexpose/variant/Variant.h>

#include <cppexpose/scripting/AbstractScriptBackend.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class QmlEngine;
class QmlObjectWrapper;


/**
*  @brief
*    Scripting backend for use in conjunction with Qt quick
*/
class GLOPERATE_QTQUICK_API QmlScriptContext : public cppexpose::AbstractScriptBackend
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

    // Virtual AbstractScriptBackend functions
    virtual void initialize(cppexpose::ScriptContext * scriptContext) override;
    virtual void setGlobalNamespace(const std::string & name) override;
    virtual void registerObject(cppexpose::PropertyGroup * obj) override;
    virtual void unregisterObject(cppexpose::PropertyGroup * obj) override;
    virtual cppexpose::Variant evaluate(const std::string & code) override;


protected:
    QmlEngine                      * m_engine;         ///< Qml engine with gloperate integration
    std::vector<QmlObjectWrapper*>   m_wrappedObjects; ///< List of wrapped objects owned by the script context
};


} // namespace gloperate_qtquick
