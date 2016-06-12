
#include <gloperate-qtquick/scripting/QmlScriptFunction.h>

#include <gloperate-qtquick/scripting/QmlEngine.h>


namespace gloperate_qtquick
{


QmlScriptFunction::QmlScriptFunction(QmlEngine * engine, QJSValue func)
: cppexpose::AbstractFunction()
, m_engine(engine)
, m_function(func)
{
}

QmlScriptFunction::~QmlScriptFunction()
{
}

cppexpose::AbstractFunction * QmlScriptFunction::clone()
{
    return new QmlScriptFunction(m_engine, m_function);
}

cppexpose::Variant QmlScriptFunction::call(const std::vector<cppexpose::Variant> & args)
{
    QJSValueList argv;

    for (cppexpose::Variant var : args) {
        argv.append(m_engine->toScriptValue(var));
    }

    if (m_function.isCallable()) {
        return m_engine->fromScriptValue(m_function.call(argv));
    } else {
        return cppexpose::Variant();
    }
}


} // namespace gloperate_qtquick
