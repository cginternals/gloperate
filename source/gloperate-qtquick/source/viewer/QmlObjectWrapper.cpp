
#include <gloperate-qtquick/viewer/QmlObjectWrapper.h>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>


using namespace cppexpose;


namespace gloperate_qtquick
{


QmlObjectWrapper::QmlObjectWrapper(QmlEngine * engine, cppexpose::PropertyGroup * group)
: QObject(engine)
, m_engine(engine)
, m_group(group)
, m_object(nullptr)
{
    // Check if a property group or an object has been provided
    m_object = dynamic_cast<cppexpose::Object *>(m_group);
}

QmlObjectWrapper::~QmlObjectWrapper()
{
}

QJSValue QmlObjectWrapper::wrapObject()
{
    // Create a nice javascript wrapper object
    QJSValue obj = m_engine->newObject();

    // Make internal object wrapper available as '_obj'
    QJSValue internal = m_engine->newQObject(this);
    obj.setProperty("_obj", internal);

    // Helper script for adding properties and functions
    QJSValue registerProperty = m_engine->evaluate("(function(obj, name) { obj.__defineGetter__(name, function() { return obj._obj.getProp(name); }); obj.__defineSetter__(name, function(v) { obj._obj.setProp(name, v); }); })");
    QJSValue registerFunction = m_engine->evaluate("(function(obj, name) { obj[name] = function() { var args = []; for (var i=0; i<arguments.length; i++) { args.push(arguments[i]); } return obj._obj.callFunc(name, args); }; })");

    // Add properties to object
    for (unsigned int i=0; i<m_group->numSubValues(); i++)
    {
        AbstractProperty * property = m_group->property(i);

        if (property->isGroup()) {
            // Add group wrapper
            PropertyGroup * group = dynamic_cast<PropertyGroup *>(property);
            QmlObjectWrapper * wrapper = new QmlObjectWrapper(m_engine, group);
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
        const std::vector<Function> funcs = m_object->functions();
        for (std::vector<Function>::const_iterator it = funcs.begin(); it != funcs.end(); ++it)
        {
            Function func = *it;

            QJSValueList args;
            args << obj;
            args << QString::fromStdString(func.name());
            registerFunction.call(args);
        }
    }

    // Return wrapper object
    return obj;
}

QJSValue QmlObjectWrapper::getProp(const QString & name)
{
    cppexpose::Variant value;

    cppexpose::AbstractProperty * prop = m_group->property(name.toStdString());
    if (prop) {
        value = prop->asTyped()->toVariant();
    }

    return m_engine->toScriptValue(value);
}

void QmlObjectWrapper::setProp(const QString & name, const QJSValue & value)
{
    cppexpose::AbstractProperty * prop = m_group->property(name.toStdString());
    if (prop) {
        prop->asTyped()->fromVariant(m_engine->fromScriptValue(value));
    }
}

QJSValue QmlObjectWrapper::callFunc(const QString & name, const QJSValue & args)
{
    // Must wrap a valid object
    if (!m_object)
    {
        return QJSValue();
    }

    // Get function
    // [TODO] Fast lookup
    cppexpose::Function function;
    for (cppexpose::Function func : m_object->functions())
    {
        if (func.name() == name.toStdString())
        {
            function = func;
            break;
        }
    }

    // Get function arguments as array
    cppexpose::VariantArray emptyArgs;
    cppexpose::Variant value = m_engine->fromScriptValue(args);
    cppexpose::VariantArray * argList = value.asArray();
    if (!argList) {
        argList = &emptyArgs;
    }

    // Call function
    return m_engine->toScriptValue(function.call(*argList));
}


} // namespace gloperate_qtquick
