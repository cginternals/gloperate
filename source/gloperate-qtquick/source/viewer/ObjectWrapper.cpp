
#include <gloperate-qtquick/viewer/ObjectWrapper.h>

#include <globjects/base/baselogging.h>

#include <reflectionzeug/Object.h>
#include <reflectionzeug/variant/Variant.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>


using namespace reflectionzeug;


namespace gloperate_qtquick
{


ObjectWrapper::ObjectWrapper(QmlEngine * engine, reflectionzeug::PropertyGroup * group)
: QObject(engine)
, m_engine(engine)
, m_group(group)
, m_object(nullptr)
{
    // Check if a property group or an object has been provided
    m_object = dynamic_cast<reflectionzeug::Object *>(m_group);
}

ObjectWrapper::~ObjectWrapper()
{
    // Clear wrapped objects
    for (ObjectWrapper * obj : m_wrappedObjects)
    {
        delete obj;
    }
}

QJSValue ObjectWrapper::wrapObject()
{
    // Create a nice javascript wrapper object
    QJSValue obj = m_engine->newObject();

    // Make internal object wrapper available as '_obj'
    QJSValue internal = m_engine->newQObject(this);
    obj.setProperty("_obj", internal);

    // Helper script for adding properties and functions
    QJSValue registerProperty = m_engine->execute("(function(obj, name) { obj.__defineGetter__(name, function() { return obj._obj.getProp(name); }); obj.__defineSetter__(name, function(v) { obj._obj.setProp(name, v); }); })");
    QJSValue registerFunction = m_engine->execute("(function(obj, name) { obj[name] = function() { var args = []; for (var i=0; i<arguments.length; i++) { args.push(arguments[i]); } return obj._obj.callFunc(name, args); }; })");

    // Add properties to object
    for (unsigned int i=0; i<m_group->count(); i++)
    {
        AbstractProperty * property = m_group->at(i);

        if (property->isGroup()) {
            // Add group wrapper
            PropertyGroup * group = property->asGroup();
            ObjectWrapper * wrapper = new ObjectWrapper(m_engine, group);
            m_wrappedObjects.push_back(wrapper);
            QJSValue groupObj = wrapper->wrapObject();
            obj.setProperty(group->name().c_str(), groupObj);
        } else {
            // Add property
            QJSValueList args;
            args << obj;
            args << QString::fromStdString(property->name());
            registerProperty.call(args);
        }
    }

    // Add functions to object
    if (m_object)
    {
        const std::vector<AbstractFunction *> & funcs = m_object->functions();
        for (std::vector<AbstractFunction *>::const_iterator it = funcs.begin(); it != funcs.end(); ++it)
        {
            AbstractFunction * func = *it;

            QJSValueList args;
            args << obj;
            args << QString::fromStdString(func->name());
            registerFunction.call(args);
        }
    }

    // Return wrapper object
    return obj;
}

QJSValue ObjectWrapper::getProp(const QString & name)
{
    reflectionzeug::Variant value;

    reflectionzeug::AbstractProperty * prop = m_group->property(name.toStdString());
    if (prop) {
        value = prop->toVariant();
    }

    return m_engine->toScriptValue(value);
}

void ObjectWrapper::setProp(const QString & name, const QJSValue & value)
{
    reflectionzeug::AbstractProperty * prop = m_group->property(name.toStdString());
    if (prop) {
        prop->fromVariant(m_engine->fromScriptValue(value));
    }
}

QJSValue ObjectWrapper::callFunc(const QString & name, const QJSValue & args)
{
    // Must wrap a valid object
    if (!m_object)
    {
        return QJSValue();
    }

    // Get function
    // [TODO] Fast lookup
    reflectionzeug::AbstractFunction * function = nullptr;
    for (reflectionzeug::AbstractFunction * func : m_object->functions())
    {
        if (func->name() == name.toStdString())
        {
            function = func;
            break;
        }
    }

    // Check if function has been found
    if (!function)
    {
        return QJSValue();
    }

    // Get function arguments as array
    reflectionzeug::VariantArray emptyArgs;
    reflectionzeug::Variant value = m_engine->fromScriptValue(args);
    reflectionzeug::VariantArray * argList = value.asArray();
    if (!argList) {
        argList = &emptyArgs;
    }

    // Call function
    return m_engine->toScriptValue(function->call(*argList));
}


} // namespace gloperate_qtquick
