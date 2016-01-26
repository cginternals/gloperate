#pragma once

#include <QString>

class JsonParseError;

class JsonParseErrorLog
{
public:
    static void error(const JsonParseError & error);

protected:
    JsonParseErrorLog();

    static QString errorMessage(const JsonParseError & error);
};
