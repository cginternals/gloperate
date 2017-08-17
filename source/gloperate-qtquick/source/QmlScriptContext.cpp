
#include <gloperate-qtquick/QmlScriptContext.h>

#include <QQmlContext>
#include <QQmlExpression>
#include <QString>
#include <QVariant>
#include <QJSValue>

#include <cppassist/logging/logging.h>

#include <cppexpose/reflection/Object.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QmlObjectWrapper.h>


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
    // m_globalObjWrapper is deleted through the Qt object hierarchy
}

void QmlScriptContext::initialize(cppexpose::ScriptContext * scriptContext)
{
    m_scriptContext = scriptContext;
}

void QmlScriptContext::addGlobalObject(cppexpose::Object * obj)
{
    // Check if obj exists
    if (m_globalObjWrappers.contains(obj))
    {
        return;
    }

    // Create object wrapper
    const auto globalObjWrapper = new QmlObjectWrapper(m_engine, obj);
    m_globalObjWrappers[obj] = globalObjWrapper;

    // Add global object
    m_engine->rootContext()->setContextProperty(QString::fromStdString(obj->name()), QVariant::fromValue(globalObjWrapper->wrapObject()));
}

void QmlScriptContext::removeGlobalObject(cppexpose::Object * obj)
{
    // Check if obj exists
    if (!m_globalObjWrappers.contains(obj))
    {
        return;
    }

    // Remove global object by setting it to null
    m_engine->rootContext()->setContextProperty(QString::fromStdString(obj->name()), QVariant{});

    // Destroy object wrapper
    delete m_globalObjWrappers[obj];
    m_globalObjWrappers.remove(obj);
}

Variant QmlScriptContext::evaluate(const std::string & code)
{
    QQmlExpression expr(m_engine->rootContext(), nullptr, QString::fromStdString(code));

    return m_engine->fromQVariant(
        expr.evaluate()
    );
}


} // namespace gloperate_qtquick
