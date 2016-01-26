#include "JsonParseErrorLog.h"

#include <QDebug>

#include "JsonParseError.h"

JsonParseErrorLog::JsonParseErrorLog()
{
}

void JsonParseErrorLog::error(const JsonParseError & error)
{
    qDebug() << "ERROR: " << errorMessage(error);
}

QString JsonParseErrorLog::errorMessage(const JsonParseError & error)
{
    switch (error.type())
    {
    case JsonParseError::DocumentNotAnObjectOrArray:
        return QString("The document is not an object or an array.");
    case JsonParseError::PropertyNotFoundOrNotAnObject:
        return QString("Property '%1' was not found or is not an object.").arg(error.info());
    case JsonParseError::PropertyNotFoundOrWrongFormat:
        return QString("Property '%1' was not found or has the wrong format.").arg(error.info());
    case JsonParseError::PropertyWrongFormat:
        return QString("Property '%1' has the wrong format.").arg(error.info());
    case JsonParseError::ElementNotObject:
        return QString("One of the elements of array '%1' is not an object.").arg(error.info());
    case JsonParseError::ElementWrongFormat:
        return QString("One of the elements of array '%1' has the wrong format.").arg(error.info());
    case JsonParseError::ArrayNotFoundOrEmpty:
        return QString("Array '%1' was not found or is empty.").arg(error.info());
    case JsonParseError::ContextCreationFailed:
        return QString("Context creation failed.");
    case JsonParseError::ContextActivationFailed:
        return QString("Context activation failed.");
    case JsonParseError::NoFilesWithExtensionFound:
        return QString("No files in directory '%1' were found with the given extensions.").arg(error.info());
    case JsonParseError::ShaderTypeNotFound:
        return QString("Shader type '%1' not found.").arg(error.info());
    default:
        return QString();
    }
}
