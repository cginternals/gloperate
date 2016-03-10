
#include <gloperate-qtquick/viewer/ScriptContext.h>

#include <QString>
#include <QJSEngine>
#include <QJSValue>
#include <QJSValueIterator>

#include <reflectionzeug/property/Property.h>
#include <reflectionzeug/function/Function.h>
#include <reflectionzeug/variant/Variant.h>
#include <reflectionzeug/Object.h>


using namespace reflectionzeug;


namespace gloperate_qtquick
{


class ScriptFunction : public reflectionzeug::AbstractFunction
{
public:
    ScriptFunction(ScriptContext * scriptContext, QJSValue func)
    : AbstractFunction("")
    , m_scriptContext(scriptContext)
    , m_function(func)
    {
    }

    virtual AbstractFunction * clone()
    {
        return new ScriptFunction(m_scriptContext, m_function);
    }

    virtual Variant call(const std::vector<Variant> & args)
    {
        QJSValueList argv;

        for (Variant var : args)
        {
            argv.append(m_scriptContext->toScriptValue(var));
        }

        if (m_function.isCallable()) {
            return m_scriptContext->fromScriptValue(m_function.call(argv));
        } else {
            return Variant();
        }
    }

protected:
    ScriptContext * m_scriptContext;
    QJSValue        m_function;
};


ScriptContext::ScriptContext(scriptzeug::ScriptContext * scriptContext, QJSEngine * engine)
: scriptzeug::AbstractScriptContext(scriptContext)
, m_engine(engine)
{
}

ScriptContext::~ScriptContext()
{
}

void ScriptContext::registerObject(PropertyGroup *)
{
    // [TODO]
}

void ScriptContext::unregisterObject(PropertyGroup *)
{
    // [TODO]
}

Variant ScriptContext::evaluate(const std::string & code)
{
    return fromScriptValue(m_engine->evaluate(QString::fromStdString(code)));
}

Variant ScriptContext::fromScriptValue(const QJSValue & value)
{
    if (value.isBool()) {
        return Variant(value.toBool());
    }

    else if (value.isNumber()) {
        return Variant(value.toNumber());
    }

    else if (value.isString()) {
        return Variant(value.toString().toStdString());
    }

    else if (value.isRegExp()) {
        return Variant(value.toString().toStdString());
    }

    else if (value.isError()) {
        return Variant(value.toString().toStdString());
    }

    else if (value.isDate()) {
        return Variant(value.toString().toStdString());
    }

    else if (value.isArray()) {
        VariantArray array;

        QJSValueIterator it(value);
        while (it.next())
        {
            if (it.hasNext()) // Skip last item (length)
            {
                array.push_back(fromScriptValue(it.value()));
            }
        }

        return array;
    }

    else if (value.isObject()) {
        VariantMap obj;

        QJSValueIterator it(value);
        while (it.next())
        {
            obj[it.name().toStdString()] = fromScriptValue(it.value());
        }

        return obj;
    }

    else if (value.isCallable()) {
        // [TODO] This produces a memory leak, since the pointer to the function object will never be deleted.
        //        A solution would be to wrap a ref_ptr into the variant, but since there are also function objects
        //        which are not memory-managed (e.g., a C-function that has been passed to the scripting engine),
        //        it would be hard to determine the right use of function-variants.
        //        The script context could of course manage a list of created functions an delete them on destruction,
        //        but that would not solve the problem of "memory leak" while the program is running.
        ScriptFunction * function = new ScriptFunction(this, value);
        return Variant::fromValue<AbstractFunction *>(function);
    }

    else {
        return Variant();
    }
}

QJSValue ScriptContext::toScriptValue(const Variant & var)
{
    if (var.hasType<char>()) {
        return QJSValue(var.value<char>());
    }

    else if (var.hasType<unsigned char>()) {
        return QJSValue(var.value<unsigned char>());
    }

    else if (var.hasType<short>()) {
        return QJSValue(var.value<short>());
    }

    else if (var.hasType<unsigned short>()) {
        return QJSValue(var.value<unsigned short>());
    }

    else if (var.hasType<int>()) {
        return QJSValue(var.value<int>());
    }

    else if (var.hasType<unsigned int>()) {
        return QJSValue(var.value<unsigned int>());
    }

    else if (var.hasType<long>()) {
        return QJSValue(var.value<int>());
    }

    else if (var.hasType<unsigned long>()) {
        return QJSValue(var.value<unsigned int>());
    }

    else if (var.hasType<long long>()) {
        return QJSValue(var.value<int>());
    }

    else if (var.hasType<unsigned long long>()) {
        return QJSValue(var.value<unsigned int>());
    }

    else if (var.hasType<float>()) {
        return QJSValue(var.value<float>());
    }

    else if (var.hasType<double>()) {
        return QJSValue(var.value<double>());
    }

    else if (var.hasType<char*>()) {
        return QJSValue(var.value<char*>());
    }

    else if (var.hasType<std::string>()) {
        return QJSValue(var.value<std::string>().c_str());
    }

    else if (var.hasType<bool>()) {
        return QJSValue(var.value<bool>());
    }

    else if (var.hasType<FilePath>()) {
        return QJSValue(var.value<FilePath>().toString().c_str());
    }

    else if (var.hasType<VariantArray>()) {
        QJSValue array = m_engine->newArray();

        VariantArray variantArray = var.value<VariantArray>();
        for (unsigned int i=0; i<variantArray.size(); i++) {
            array.setProperty(i, toScriptValue(variantArray.at(i)));
        }

        return array;
    }

    else if (var.hasType<VariantMap>()) {
        QJSValue obj = m_engine->newObject();

        VariantMap variantMap = var.value<VariantMap>();
        for (const std::pair<std::string, Variant> & pair : variantMap)
        {
            obj.setProperty(pair.first.c_str(), toScriptValue(pair.second));
        }

        return obj;
    }

    else {
        return QJSValue();
    }
}


} // namespace gloperate_qtquick
