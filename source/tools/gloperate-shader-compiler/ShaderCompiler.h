#pragma once

#include <set>
#include <string>
#include <vector>

#include <QString>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include "JsonParseError.h"


class QOffscreenSurface;
class QOpenGLContext;
class QJsonArray;
class QJsonObject;
class QJsonDocument;
class QSurfaceFormat;

namespace globjects
{

class Shader;
class AbstractStringSource;

}

class ShaderCompiler
{
public:
    ShaderCompiler(const QString & basePath);

    static bool process(const QString & basePath, const QJsonDocument & configDocument);
    
private:
    bool parse(const QJsonDocument & configDocument);
    bool parse(const QJsonArray & config);
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
    
    bool replaceStrings(
        const QJsonObject & replacements,
        globjects::ref_ptr<globjects::AbstractStringSource> & stringSource);
    
    static gl::GLenum typeFromString(const QString & typeString);

    bool createAndLinkProgram(
        const std::vector<globjects::ref_ptr<globjects::Shader>> & shadersArray);
    
private:
    enum class Info
    {
        Driver,
        Failures
    };

    void info(Info type);
    
    void error(JsonParseError error);
    void error(JsonParseError::Type type, const QString & info);
    
private:
    QString m_basePath;
    std::vector<std::string> m_compileFailures;
    std::vector<std::string> m_linkFailures;
};
