
#include <gloperate-qtquick/viewer/QmlEngine.h>

#include <QVariant>
#include <QQmlContext>
#include <QJSValueIterator>

#include <cppexpose/reflection/Property.h>

#include <iozeug/FilePath.h>

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

cppexpose::Variant QmlEngine::fromScriptValue(const QJSValue & value)
{
    if (value.isBool()) {
        return cppexpose::Variant(value.toBool());
    }

    else if (value.isNumber()) {
        return cppexpose::Variant(value.toNumber());
    }

    else if (value.isString()) {
        return cppexpose::Variant(value.toString().toStdString());
    }

    else if (value.isRegExp()) {
        return cppexpose::Variant(value.toString().toStdString());
    }

    else if (value.isError()) {
        return cppexpose::Variant(value.toString().toStdString());
    }

    else if (value.isDate()) {
        return cppexpose::Variant(value.toString().toStdString());
    }

    else if (value.isCallable()) {
        // [TODO] This produces a memory leak, since the pointer to the function object will never be deleted.
        //        A solution would be to wrap a ref_ptr into the variant, but since there are also function objects
        //        which are not memory-managed (e.g., a C-function that has been passed to the scripting engine),
        //        it would be hard to determine the right use of function-variants.
        //        The script context could of course manage a list of created functions an delete them on destruction,
        //        but that would not solve the problem of "memory leak" while the program is running.
        QmlScriptFunction * function = new QmlScriptFunction(this, value);
        return cppexpose::Variant::fromValue<cppexpose::AbstractFunction *>(function);
    }

    else if (value.isArray()) {
        cppexpose::VariantArray array;

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
        cppexpose::VariantMap obj;

        QJSValueIterator it(value);
        while (it.next())
        {
            obj[it.name().toStdString()] = fromScriptValue(it.value());
        }

        return obj;
    }

    else {
        return cppexpose::Variant();
    }
}

QJSValue QmlEngine::toScriptValue(const cppexpose::Variant & var)
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

    else if (var.hasType<iozeug::FilePath>()) {
        return QJSValue(var.value<iozeug::FilePath>().path().c_str());
    }

    else if (var.hasType<cppexpose::VariantArray>()) {
        QJSValue array = newArray();

        cppexpose::VariantArray variantArray = var.value<cppexpose::VariantArray>();
        for (unsigned int i=0; i<variantArray.size(); i++) {
            array.setProperty(i, toScriptValue(variantArray.at(i)));
        }

        return array;
    }

    else if (var.hasType<cppexpose::VariantMap>()) {
        QJSValue obj = newObject();

        cppexpose::VariantMap variantMap = var.value<cppexpose::VariantMap>();
        for (const std::pair<std::string, cppexpose::Variant> & pair : variantMap)
        {
            obj.setProperty(pair.first.c_str(), toScriptValue(pair.second));
        }

        return obj;
    }

    else {
        return QJSValue();
    }
}

cppexpose::Variant QmlEngine::fromQVariant(const QVariant & value)
{
    if (value.type() == QVariant::Bool) {
        return cppexpose::Variant(value.toBool());
    }

    else if (value.type() == QVariant::Char || value.type() == QVariant::Int) {
        return cppexpose::Variant(value.toInt());
    }

    else if (value.type() == QVariant::UInt) {
        return cppexpose::Variant(value.toUInt());
    }

    else if (value.type() == QVariant::LongLong) {
        return cppexpose::Variant(value.toLongLong());
    }

    else if (value.type() == QVariant::ULongLong) {
        return cppexpose::Variant(value.toULongLong());
    }

    else if (value.type() == QVariant::Double) {
        return cppexpose::Variant(value.toDouble());
    }

    else if (value.type() == QVariant::StringList)
    {
        cppexpose::VariantArray array;

        QStringList list = value.toStringList();
        for (QStringList::iterator it = list.begin(); it != list.end(); ++it)
        {
            array.push_back( cppexpose::Variant((*it).toStdString()) );
        }

        return array;
    }

    else if (value.type() == QVariant::List)
    {
        cppexpose::VariantArray array;

        QList<QVariant> list = value.toList();
        for (QList<QVariant>::iterator it = list.begin(); it != list.end(); ++it)
        {
            array.push_back(fromQVariant(*it));
        }

        return array;
    }

    else if (value.type() == QVariant::Map)
    {
        cppexpose::VariantMap obj;

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
        return cppexpose::Variant(value.toString().toStdString());
    }

    else {
        return cppexpose::Variant();
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
