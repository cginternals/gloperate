
#include <gloperate-qtquick/scripting/QmlObjectWrapper.h>

#include <QJSValueIterator>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/reflection/Method.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate-qtquick/scripting/QmlEngine.h>


using namespace cppexpose;


static const char * s_registerProperty = R"(
    (function(obj, name)
     {
        obj.__defineGetter__(name, function()
        {
            return obj._obj.getProp(name);
        } );

        obj.__defineSetter__(name, function(v)
        {
            obj._obj.setProp(name, v);
        } );
    } )
)";

static const char * s_registerFunction = R"(
    (function(obj, name)
    {
        obj[name] = function()
        {
            var args = [];

            for (var i=0; i<arguments.length; i++)
            {
                args.push(arguments[i]);
            }

            return obj._obj.callFunc(name, args);
        };
    } )
)";


namespace gloperate_qtquick
{


QmlObjectWrapper::QmlObjectWrapper(QmlEngine * engine, cppexpose::Object * obj)
: m_engine(engine)
, m_object(obj)
{
}

QmlObjectWrapper::~QmlObjectWrapper()
{
    // Wrapped objects are deleted through the Qt object hierarchy
}

QJSValue QmlObjectWrapper::wrapObject()
{
    // Create a nice javascript wrapper object
    m_obj = m_engine->newObject();

    // Make internal object wrapper available as '_obj'
    QJSValue internal = m_engine->newQObject(this);
    m_obj.setProperty("_obj", internal);

    // Helper script for adding properties and functions
    m_registerProperty = m_engine->evaluate(s_registerProperty);
    m_registerFunction = m_engine->evaluate(s_registerFunction);

    // Add properties to object
    for (unsigned int i=0; i<m_object->numSubValues(); i++)
    {
        AbstractProperty * property = m_object->property(i);

        if (property->isObject()) {
            // Add object wrapper
            Object * obj = static_cast<Object *>(property);

            QmlObjectWrapper * wrapper = new QmlObjectWrapper(m_engine, obj);
            m_wrappedObjects.push_back(wrapper);

            QJSValue wrapObj = wrapper->wrapObject();
            m_obj.setProperty(obj->name().c_str(), wrapObj);
        } else {
            // Add property
            QJSValueList args;
            args << m_obj;
            args << QString::fromStdString(property->name());
            m_registerProperty.call(args);

            m_wrappedObjects.push_back(nullptr);
        }
    }

    // Add functions to object
    const std::vector<Method> funcs = m_object->functions();
    for (std::vector<Method>::const_iterator it = funcs.begin(); it != funcs.end(); ++it)
    {
        const Method & func = *it;

        QJSValueList args;
        args << m_obj;
        args << QString::fromStdString(func.name());
        m_registerFunction.call(args);
    }

    // Register callbacks for script engine update
    m_beforeDestroyConnection = m_object->beforeDestroy.connect([this](cppexpose::AbstractProperty * property)
    {
        // [TODO] Provide an UNUSED() macro in cppassist
        assert(property == m_object);
        (void)(property);

        // Clear wrapper object
        // [TODO] How to detect the point in execution where m_obj gets invalid?
        QJSValueIterator it(m_obj);
        while (it.hasNext()) {
            it.next();
            m_obj.deleteProperty(it.name());
        }
    });

    m_afterAddConnection = m_object->afterAdd.connect([this](size_t index, cppexpose::AbstractProperty * property)
    {
        // [TODO] Provide an UNUSED() macro in cppassist
        (void)(index);

        // Add property to object
        if (property->isObject())
        {
            // Add object wrapper
            Object * obj = static_cast<Object *>(property);
            QmlObjectWrapper * wrapper = new QmlObjectWrapper(m_engine, obj);

            assert(m_wrappedObjects.size() == index);

            m_wrappedObjects.push_back(wrapper);

            QJSValue wrapObj = wrapper->wrapObject();
            m_obj.setProperty(obj->name().c_str(), wrapObj);
        }
        else
        {
            // Add property
            QJSValueList args;
            args << m_obj;
            args << QString::fromStdString(property->name());
            m_registerProperty.call(args);

            m_wrappedObjects.push_back(nullptr);
        }
    });

    m_beforeRemoveConnection = m_object->beforeRemove.connect([this](size_t index, cppexpose::AbstractProperty * property)
    {
        // Remove object
        m_obj.deleteProperty(QString::fromStdString(property->name()));
        delete *(m_wrappedObjects.begin() + index);
        m_wrappedObjects.erase(m_wrappedObjects.begin() + index);

        // [TODO] remove object wrapper from parent wrapper? -> currently a memory leak
    });

    // Return wrapper object
    return m_obj;
}

QJSValue QmlObjectWrapper::getProp(const QString & name)
{
    cppexpose::Variant value;

    cppexpose::AbstractProperty * prop = m_object->property(name.toStdString());
    if (prop) {
        value = prop->toVariant();
    }

    return m_engine->toScriptValue(value);
}

void QmlObjectWrapper::setProp(const QString & name, const QJSValue & value)
{
    cppexpose::AbstractProperty * prop = m_object->property(name.toStdString());
    if (prop) {
        prop->fromVariant(m_engine->fromScriptValue(value));
    }
}

QJSValue QmlObjectWrapper::callFunc(const QString & name, const QJSValue & args)
{
    // Get function
    // [TODO] Fast lookup
    cppexpose::Function function;
    for (const cppexpose::Method & func : m_object->functions())
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
