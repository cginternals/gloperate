
#include <gloperate-qtquick/viewer/QmlEngine.h>

#include <QQmlContext>
#include <QJSValueIterator>

#include <reflectionzeug/property/Property.h>

#include <gloperate/gloperate.h>

#include <gloperate-qtquick/controls/TextController.h>
#include <gloperate-qtquick/viewer/RenderItem.h>
#include <gloperate-qtquick/viewer/ScriptFunction.h>

    
namespace gloperate_qtquick
{


QmlEngine::QmlEngine(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
    // Register QML types
    qmlRegisterType<RenderItem>    ("gloperate.rendering", 1, 0, "RenderItem");
    qmlRegisterType<TextController>("gloperate.ui",        1, 0, "TextController");

    // Add gloperate qml-libraries
    std::string importPath = gloperate::dataPath() + "/gloperate/qml/GLOperate/Ui";
    addImportPath(QString::fromStdString(importPath));

    // Register global functions and properties
    rootContext()->setContextObject(this);

    // Create global objects
    m_global    = newObject();
    m_gloperate = newObject();
}

QmlEngine::~QmlEngine()
{
}

gloperate::ViewerContext * QmlEngine::viewerContext() const
{
    return m_viewerContext;
}

QJSValue QmlEngine::execute(const QString & code)
{
    return evaluate(code);
}

reflectionzeug::Variant QmlEngine::fromScriptValue(const QJSValue & value)
{
    if (value.isBool()) {
        return reflectionzeug::Variant(value.toBool());
    }

    else if (value.isNumber()) {
        return reflectionzeug::Variant(value.toNumber());
    }

    else if (value.isString()) {
        return reflectionzeug::Variant(value.toString().toStdString());
    }

    else if (value.isRegExp()) {
        return reflectionzeug::Variant(value.toString().toStdString());
    }

    else if (value.isError()) {
        return reflectionzeug::Variant(value.toString().toStdString());
    }

    else if (value.isDate()) {
        return reflectionzeug::Variant(value.toString().toStdString());
    }

    else if (value.isArray()) {
        reflectionzeug::VariantArray array;

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
        reflectionzeug::VariantMap obj;

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
        return reflectionzeug::Variant::fromValue<reflectionzeug::AbstractFunction *>(function);
    }

    else {
        return reflectionzeug::Variant();
    }
}

QJSValue QmlEngine::toScriptValue(const reflectionzeug::Variant & var)
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

    else if (var.hasType<reflectionzeug::FilePath>()) {
        return QJSValue(var.value<reflectionzeug::FilePath>().toString().c_str());
    }

    else if (var.hasType<reflectionzeug::VariantArray>()) {
        QJSValue array = newArray();

        reflectionzeug::VariantArray variantArray = var.value<reflectionzeug::VariantArray>();
        for (unsigned int i=0; i<variantArray.size(); i++) {
            array.setProperty(i, toScriptValue(variantArray.at(i)));
        }

        return array;
    }

    else if (var.hasType<reflectionzeug::VariantMap>()) {
        QJSValue obj = newObject();

        reflectionzeug::VariantMap variantMap = var.value<reflectionzeug::VariantMap>();
        for (const std::pair<std::string, reflectionzeug::Variant> & pair : variantMap)
        {
            obj.setProperty(pair.first.c_str(), toScriptValue(pair.second));
        }

        return obj;
    }

    else {
        return QJSValue();
    }
}

const QJSValue & QmlEngine::global() const
{
    return m_global;
}

void QmlEngine::setGlobal(const QJSValue & obj)
{
    m_global = obj;
}

const QJSValue & QmlEngine::gloperate() const
{
    return m_gloperate;
}

void QmlEngine::setGloperate(const QJSValue & obj)
{
    m_gloperate = obj;
}


} // namespace gloperate_qtquick
