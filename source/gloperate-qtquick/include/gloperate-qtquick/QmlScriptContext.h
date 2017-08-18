
#pragma once


#include <QMap>

#include <cppexpose/scripting/AbstractScriptBackend.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace cppexpose
{
    class Variant;
}


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
    virtual void addGlobalObject(cppexpose::Object * obj) override;
    virtual void removeGlobalObject(cppexpose::Object * obj) override;
    virtual cppexpose::Variant evaluate(const std::string & code) override;


protected:
    QmlEngine                                   * m_engine;            ///< Qml engine with gloperate integration
    QMap<cppexpose::Object *, QmlObjectWrapper *> m_globalObjWrappers; ///< Global objects
};


} // namespace gloperate_qtquick
