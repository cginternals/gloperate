
#include <gloperate-qtquick/viewer/QmlEngine.h>

#include <QVariant>
#include <QQmlContext>
#include <QJSValueIterator>

#include <reflectionzeug/property/Property.h>

#include <gloperate/gloperate.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/scripting/ScriptEnvironment.h>

#include <gloperate-qtquick/controls/TextController.h>
#include <gloperate-qtquick/viewer/RenderItem.h>
#include <gloperate-qtquick/viewer/QmlScriptFunction.h>

    
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

QString QmlEngine::execute(const QString & code)
{
    return QString::fromStdString(
        m_viewerContext->scriptEnvironment()->execute(code.toStdString()).value<std::string>()
    );
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

    else if (value.isCallable()) {
        // [TODO] This produces a memory leak, since the pointer to the function object will never be deleted.
        //        A solution would be to wrap a ref_ptr into the variant, but since there are also function objects
        //        which are not memory-managed (e.g., a C-function that has been passed to the scripting engine),
        //        it would be hard to determine the right use of function-variants.
        //        The script context could of course manage a list of created functions an delete them on destruction,
        //        but that would not solve the problem of "memory leak" while the program is running.
        QmlScriptFunction * function = new QmlScriptFunction(this, value);
        return reflectionzeug::Variant::fromValue<reflectionzeug::AbstractFunction *>(function);
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

reflectionzeug::Variant QmlEngine::fromQVariant(const QVariant & value)
{
    if (value.type() == QVariant::Bool) {
        return reflectionzeug::Variant(value.toBool());
    }

    else if (value.type() == QVariant::Char || value.type() == QVariant::Int) {
        return reflectionzeug::Variant(value.toInt());
    }

    else if (value.type() == QVariant::UInt) {
        return reflectionzeug::Variant(value.toUInt());
    }

    else if (value.type() == QVariant::LongLong) {
        return reflectionzeug::Variant(value.toLongLong());
    }

    else if (value.type() == QVariant::ULongLong) {
        return reflectionzeug::Variant(value.toULongLong());
    }

    else if (value.type() == QVariant::Double) {
        return reflectionzeug::Variant(value.toDouble());
    }

    else if (value.type() == QVariant::StringList)
    {
        reflectionzeug::VariantArray array;

        QStringList list = value.toStringList();
        for (QStringList::iterator it = list.begin(); it != list.end(); ++it)
        {
            array.push_back( reflectionzeug::Variant((*it).toStdString()) );
        }

        return array;
    }

    else if (value.type() == QVariant::List)
    {
        reflectionzeug::VariantArray array;

        QList<QVariant> list = value.toList();
        for (QList<QVariant>::iterator it = list.begin(); it != list.end(); ++it)
        {
            array.push_back(fromQVariant(*it));
        }

        return array;
    }

    else if (value.type() == QVariant::Map)
    {
        reflectionzeug::VariantMap obj;

        QMap<QString, QVariant> map = value.toMap();
        for (QMap<QString, QVariant>::iterator it = map.begin(); it != map.end(); ++it)
        {
            std::string key = it.key().toStdString();
            obj[key] = fromQVariant(it.value());
        }

        return obj;
    }

    else if (value.type() == QVariant::String || value.canConvert(QVariant::String))
    {
        return reflectionzeug::Variant(value.toString().toStdString());
    }

    else {
        return reflectionzeug::Variant();
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
