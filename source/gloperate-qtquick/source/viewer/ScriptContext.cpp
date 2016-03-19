
#include <gloperate-qtquick/viewer/ScriptContext.h>

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
}

void ScriptContext::registerObject(PropertyGroup * group)
{
    // [TODO]
    reflectionzeug::Object * obj = static_cast<reflectionzeug::Object*>(group);
    ObjectWrapper * wrapper = new ObjectWrapper(m_engine, obj);

    QJSValue value = m_engine->newQObject(wrapper);
    m_engine->rootContext()->setContextProperty(obj->name().c_str(), value.toVariant());
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
