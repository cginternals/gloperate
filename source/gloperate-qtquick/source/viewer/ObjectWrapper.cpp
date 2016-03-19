
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
, m_object(object)
{
}

ObjectWrapper::~ObjectWrapper()
{
}

QJSValue ObjectWrapper::getProp(const QString & name)
{
    reflectionzeug::Variant value;

    reflectionzeug::AbstractProperty * prop = m_object->property(name.toStdString());
    if (prop) {
        value = prop->toVariant();
    }

    return m_engine->toScriptValue(value);
}

void ObjectWrapper::setProp(const QString & name, const QJSValue & value)
{
    reflectionzeug::AbstractProperty * prop = m_object->property(name.toStdString());
    if (prop) {
        prop->fromVariant(m_engine->fromScriptValue(value));
    }
}

QJSValue ObjectWrapper::callFunc(const QString & name, const QJSValue & args)
{
//    for (QJSValue arg : args)
//    {
    reflectionzeug::Variant value = m_engine->fromScriptValue(args);
    globjects::info() << value.value<std::string>();

    QJSValue ret;
    return ret;
//    }
}


} // namespace gloperate_qtquick
