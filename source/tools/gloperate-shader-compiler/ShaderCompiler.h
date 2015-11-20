#pragma once

#include <string>

#include <QList>
#include <QString>

class ShaderCompiler
{
public:
    ShaderCompiler(const QList<QString> & includePathsAndAliases);

    bool compile(const QString & shaderFile);
protected:
    bool compileFile(const std::string & shaderFile, const std::string & extension);
};
