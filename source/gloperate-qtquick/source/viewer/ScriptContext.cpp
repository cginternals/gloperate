
#include <gloperate-qtquick/viewer/ScriptContext.h>

#include <globjects/base/baselogging.h>

#include <QString>
#include <QQmlContext>
#include <QJSValue>

#include <reflectionzeug/Object.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/ObjectWrapper.h>


using namespace reflectionzeug;


namespace gloperate_qtquick
{


ScriptContext::ScriptContext(scriptzeug::ScriptContext * scriptContext, QmlEngine * engine)
: scriptzeug::AbstractScriptContext(scriptContext)
, m_engine(engine)
{
}

ScriptContext::~ScriptContext()
{
    // Clear wrapped objects
    for (ObjectWrapper * obj : m_wrappedObjects)
    {
        delete obj;
    }
}

void ScriptContext::registerObject(PropertyGroup * group)
{
    // Check arguments
    if (!group) return;

    // Create object wrapper
    ObjectWrapper * wrapper = new ObjectWrapper(m_engine, group);
    m_wrappedObjects.push_back(wrapper);

    // Add object to global object 'gloperate'
    QJSValue gloperate = m_engine->gloperate();
    gloperate.setProperty(group->name().c_str(), wrapper->wrapObject());
    m_engine->setGloperate(gloperate);
}

void ScriptContext::unregisterObject(PropertyGroup *)
{
    // [TODO]
}

Variant ScriptContext::evaluate(const std::string & code)
{
    return m_engine->fromScriptValue(m_engine->evaluate(QString::fromStdString(code)));
}


} // namespace gloperate_qtquick
