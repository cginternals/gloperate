#pragma once

#include <string>

#include <QList>
#include <QString>

class ShaderCompiler_old
{
public:
    ShaderCompiler_old(const QList<QString> & includePathsAndAliases);

    bool compile(const QString & shaderFile);
protected:
    bool compileFile(const std::string & shaderFile, const std::string & extension);
};
