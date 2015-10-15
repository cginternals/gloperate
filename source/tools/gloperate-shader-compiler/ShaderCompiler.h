#pragma once

#include <set>
#include <string>
#include <vector>

#include <QString>

#include <glbinding/gl/enum.h>

#include <globjects/base/ref_ptr.h>

#include "JsonParseErrorLog.h"


class QOffscreenSurface;
class QOpenGLContext;
class QJsonArray;
class QJsonObject;
class QSurfaceFormat;

namespace globjects
{
    class Shader;
}

class ShaderCompiler
{
public:
    static bool process(const QJsonObject & config);
    
private:    
    bool parse(const QJsonObject & config);

    bool parseNamedStringPaths(const QJsonArray & paths);
    
    std::set<std::string> parseExtensions(
        const QJsonArray & extensionsArray,
        bool & ok);
    
    std::vector<std::string> scanDirectory(
        const std::string & path,
        const std::set<std::string> & extensions);
    
    std::vector<std::string> createAliases(
        const std::vector<std::string> & files,
        const std::string & path,
        const std::string & alias);
    
    void createNamedStrings(
        const std::vector<std::string> & files,
        const std::vector<std::string> & aliases);
    
    bool parsePrograms(const QJsonArray & programs);
    
    std::vector<globjects::ref_ptr<globjects::Shader>> parseShaders(
        const QJsonArray & shaders,
        bool & ok);
    
    static gl::GLenum typeFromString(const QString & typeString);

    bool createAndLinkProgram(
        const std::vector<globjects::ref_ptr<globjects::Shader>> & shadersArray);
    
private:
    enum class Info {
        Driver,
        Failures
    };

    void info(Info type);
    
    void error(JsonParseError error);
    void error(JsonParseError::Type type, const QString & info);
    
private:
    std::vector<std::string> m_compileFailures;
    std::vector<std::string> m_linkFailures;
    JsonParseErrorLog m_errorLog;
};
