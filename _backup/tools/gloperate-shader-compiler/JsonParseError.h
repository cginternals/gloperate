#pragma once

#include <QString>

class JsonParseError
{
public:
    enum Type
    {
        NoError,
        DocumentNotAnObjectOrArray,
        PropertyNotFoundOrNotAnObject,
        PropertyNotFoundOrWrongFormat,
        PropertyWrongFormat,
        ElementNotObject,
        ElementWrongFormat,
        ArrayNotFoundOrEmpty,
        ContextCreationFailed,
        ContextActivationFailed,
        NoFilesWithExtensionFound,
        ShaderTypeNotFound
    };

public:
    JsonParseError();
    JsonParseError(Type type);
    JsonParseError(Type type, const QString & info);

    Type type() const;
    const QString & info() const;

    // @returns true iff it is an actual error
    operator bool() const;     

protected:
    Type m_type;
    QString m_info;
};
