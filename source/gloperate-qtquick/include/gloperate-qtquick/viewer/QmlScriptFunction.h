
#pragma once


#include <QJSValue>

#include <cppexpose/variant/Variant.h>
#include <cppexpose/function/AbstractFunction.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate_qtquick
{


class QmlEngine;


/**
*  @brief
*    Script function wrapper
*
*    This class represents a function inside the scripting environment
*    and makes it available to the reflection meta object system.
*/
class GLOPERATE_QTQUICK_API QmlScriptFunction : public cppexpose::AbstractFunction
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
    QmlScriptFunction(QmlEngine * engine, QJSValue func);

    /**
    *  @brief
    *    Destructor
    */
    ~QmlScriptFunction();

    // Virtual cppexpose::AbstractFunction functions
    virtual cppexpose::AbstractFunction * clone() override;
    virtual cppexpose::Variant call(const std::vector<cppexpose::Variant> & args) override;


protected:
    QmlEngine * m_engine;   ///< Qml engine with gloperate integration
    QJSValue    m_function; ///< The wrapped function object
};


} // namespace gloperate_qtquick
