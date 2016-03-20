
#include <gloperate-qtquick/viewer/ScriptFunction.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>


namespace gloperate_qtquick
{


ScriptFunction::ScriptFunction(QmlEngine * engine, QJSValue func)
: reflectionzeug::AbstractFunction("")
, m_engine(engine)
, m_function(func)
{
}

ScriptFunction::~ScriptFunction()
{
}

reflectionzeug::AbstractFunction * ScriptFunction::clone()
{
    return new ScriptFunction(m_engine, m_function);
}

reflectionzeug::Variant ScriptFunction::call(const std::vector<reflectionzeug::Variant> & args)
{
    QJSValueList argv;

    for (reflectionzeug::Variant var : args) {
        argv.append(m_engine->toScriptValue(var));
    }

    if (m_function.isCallable()) {
        return m_engine->fromScriptValue(m_function.call(argv));
    } else {
        return reflectionzeug::Variant();
    }
}


} // namespace gloperate_qtquick
