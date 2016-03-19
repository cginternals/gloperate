
#include <gloperate-qtquick/viewer/ObjectWrapper.h>

#include <globjects/base/baselogging.h>

#include <reflectionzeug/Object.h>
#include <reflectionzeug/variant/Variant.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>

    
namespace gloperate_qtquick
{


ObjectWrapper::ObjectWrapper(QmlEngine * engine, reflectionzeug::Object * object)
: QObject(engine)   // Important, otherwise the object is destroyed too early
, m_engine(engine)
, m_group(object)
, m_object(object)
{
}

ObjectWrapper::ObjectWrapper(QmlEngine * engine, reflectionzeug::PropertyGroup * group)
: QObject(engine)
, m_engine(engine)
, m_group(group)
, m_object(nullptr)
{
}

ObjectWrapper::~ObjectWrapper()
{
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
