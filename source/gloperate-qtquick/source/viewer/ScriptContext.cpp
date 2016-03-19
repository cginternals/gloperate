
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
}

void ScriptContext::registerObject(PropertyGroup * group)
{
    // Check arguments
    if (!group) return;

    // Check if a property group or an object has been provided
    reflectionzeug::Object * object = dynamic_cast<reflectionzeug::Object *>(group);

    // Create object wrapper
    ObjectWrapper * wrapper = nullptr;
    if (object) wrapper = new ObjectWrapper(m_engine, object);
    else        wrapper = new ObjectWrapper(m_engine, group);

    // Create a nice javascript wrapper object
    QJSValue obj = m_engine->newObject();

    // Make internal object wrapper available as '_obj'
    QJSValue internal = m_engine->newQObject(wrapper);
    obj.setProperty("_obj", internal);

    // Add properties to object
    QJSValue registerProperty = m_engine->execute("(function(obj, name) { obj.__defineGetter__(name, function() { return obj._obj.getProp(name); }); obj.__defineSetter__(name, function(v) { obj._obj.setProp(name, v); }); })");
    for (unsigned int i=0; i<group->count(); i++)
    {
        AbstractProperty * property = group->at(i);

        QJSValueList args;
        args << obj;
        args << QString::fromStdString(property->name());
        registerProperty.call(args);
    }

    // Add functions to object
    if (object)
    {
        QJSValue registerFunction = m_engine->execute("(function(obj, name) { obj[name] = function() { var args = []; for (var i=0; i<arguments.length; i++) { args.push(arguments[i]); } return obj._obj.callFunc(name, args); }; })");

        const std::vector<AbstractFunction *> & funcs = object->functions();
        for (std::vector<AbstractFunction *>::const_iterator it = funcs.begin(); it != funcs.end(); ++it)
        {
            AbstractFunction * func = *it;

            QJSValueList args;
            args << obj;
            args << QString::fromStdString(func->name());
            registerFunction.call(args);
        }
    }

    // Add object to global object 'gloperate'
    QJSValue gloperate = m_engine->gloperate();
    gloperate.setProperty(object->name().c_str(), obj);
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
