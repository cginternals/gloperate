#pragma once

#include <set>
#include <string>
#include <vector>

#include <glbinding/gl/enum.h>

#include <globjects/base/ref_ptr.h>


class QString;
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
    static void parseNamedStringPaths(const QJsonArray & paths);
    
    static std::set<std::string> parseExtensions(
        const QJsonArray & extensionsArray,
        bool & ok);
    
    static std::vector<std::string> scanDirectory(
        const std::string & path,
        const std::set<std::string> & extensions);
    
    static std::vector<std::string> createAliases(
        const std::vector<std::string> & files,
        const std::string & path,
        const std::string & alias);
    
    static void createNamedStrings(
        const std::vector<std::string> & files,
        const std::vector<std::string> & aliases);
    
    static bool parsePrograms(const QJsonArray & programs);
    
    static std::vector<globjects::ref_ptr<globjects::Shader>> parseShaders(
        const QJsonArray & shaders,
        bool & ok);
    
    static gl::GLenum typeFromString(const QString & typeString);

    static bool createAndLinkProgram(
        const std::vector<globjects::ref_ptr<globjects::Shader>> & shadersArray);
};
