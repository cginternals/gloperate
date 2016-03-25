
#include <gloperate-qtquick/viewer/QmlScriptContext.h>

#include <globjects/base/baselogging.h>

#include <QString>
#include <QQmlContext>
#include <QJSValue>

#include <reflectionzeug/Object.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QmlObjectWrapper.h>


using namespace reflectionzeug;


namespace gloperate_qtquick
{


QmlScriptContext::QmlScriptContext(QmlEngine * engine)
: m_engine(engine)
{
}

QmlScriptContext::~QmlScriptContext()
{
    // Clear wrapped objects
    for (QmlObjectWrapper * obj : m_wrappedObjects)
    {
        delete obj;
    }
}

void QmlScriptContext::initialize(scriptzeug::ScriptContext * scriptContext)
{
    m_scriptContext = scriptContext;
}

void QmlScriptContext::registerObject(PropertyGroup * group)
{
    // Check arguments
    if (!group) return;

    // Create object wrapper
    QmlObjectWrapper * wrapper = new QmlObjectWrapper(m_engine, group);
    m_wrappedObjects.push_back(wrapper);

    // Add object to global object 'gloperate'
    QJSValue gloperate = m_engine->gloperate();
    gloperate.setProperty(group->name().c_str(), wrapper->wrapObject());
    m_engine->setGloperate(gloperate);
}

void QmlScriptContext::unregisterObject(PropertyGroup *)
{
    // [TODO]
}

Variant QmlScriptContext::evaluate(const std::string & code)
{
    return m_engine->fromScriptValue(m_engine->evaluate(QString::fromStdString(code)));
}


} // namespace gloperate_qtquick
