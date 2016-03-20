
#pragma once


#include <QJSValue>

#include <reflectionzeug/variant/Variant.h>
#include <reflectionzeug/function/Function.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class QmlEngine;


/**
*  @brief
*    Script function wrapper
*
*    This class represents a function inside the scripting environment
*    and makes it available to the reflectionzeug meta object system.
*/
class GLOPERATE_QTQUICK_API ScriptFunction : public reflectionzeug::AbstractFunction
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] engine
    *    Qml engine
    *  @param[in] func
    *    The wrapped function object
    */
    ScriptFunction(QmlEngine * engine, QJSValue func);

    /**
    *  @brief
    *    Destructor
    */
    ~ScriptFunction();

    // Virtual reflectionzeug::AbstractFunction functions
    virtual reflectionzeug::AbstractFunction * clone() override;
    virtual reflectionzeug::Variant call(const std::vector<reflectionzeug::Variant> & args) override;


protected:
    QmlEngine * m_engine;   ///< Qml engine with gloperate integration
    QJSValue    m_function; ///< The wrapped function object
};


} // namespace gloperate_qtquick
