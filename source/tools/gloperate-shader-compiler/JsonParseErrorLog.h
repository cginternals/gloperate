#pragma once

#include <QString>

#include "JsonParseError.h"


class JsonParseErrorLog
{
public:
    JsonParseErrorLog() = default;

    void error(const JsonParseError & error);

private:
    QString errorMessage(const JsonParseError & error);
};
