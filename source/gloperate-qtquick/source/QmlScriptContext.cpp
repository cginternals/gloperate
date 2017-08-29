
#include <gloperate-qtquick/QmlScriptContext.h>

#include <QQmlContext>
#include <QQmlExpression>
#include <QString>
#include <QVariant>
#include <QJSValue>

#include <cppassist/logging/logging.h>

#include <cppexpose/reflection/Object.h>

#include <gloperate-qtquick/QmlEngine.h>


using namespace cppassist;
using namespace cppexpose;


namespace gloperate_qtquick
{


QmlScriptContext::QmlScriptContext(QmlEngine * engine)
: m_engine(engine)
{
}

QmlScriptContext::~QmlScriptContext()
{
}

void QmlScriptContext::initialize(cppexpose::ScriptContext * scriptContext)
{
    m_scriptContext = scriptContext;
}

void QmlScriptContext::addGlobalObject(cppexpose::Object * obj)
{
    m_engine->addGlobalObject(obj);
}

void QmlScriptContext::removeGlobalObject(cppexpose::Object * obj)
{
    m_engine->removeGlobalObject(obj);
}

Variant QmlScriptContext::evaluate(const std::string & code)
{
    QQmlExpression expr(m_engine->rootContext(), nullptr, QString::fromStdString(code));

    return m_engine->fromQVariant(
        expr.evaluate()
    );
}


} // namespace gloperate_qtquick
