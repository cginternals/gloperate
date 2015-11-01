#pragma once

#include "QString"


class JsonParseError {
public:
    enum Type {
        NoError,
        DocumentNotAnObject,
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
    QString info() const;

    operator bool() const;     

private:
    Type m_type;
    QString m_info;
};
