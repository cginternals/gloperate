
#include <gloperate-qtquick/viewer/QmlScriptFunction.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>


namespace gloperate_qtquick
{


QmlScriptFunction::QmlScriptFunction(QmlEngine * engine, QJSValue func)
: reflectionzeug::AbstractFunction("")
, m_engine(engine)
, m_function(func)
{
}

QmlScriptFunction::~QmlScriptFunction()
{
}

reflectionzeug::AbstractFunction * QmlScriptFunction::clone()
{
    return new QmlScriptFunction(m_engine, m_function);
}

reflectionzeug::Variant QmlScriptFunction::call(const std::vector<reflectionzeug::Variant> & args)
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
