#pragma once

#include <set>
#include <string>
#include <vector>

#include <globjects/base/ref_ptr.h>


class QOffscreenSurface;
class QOpenGLContext;
class QJsonArray;
class QJsonObject;
class QSurfaceFormat;

namespace globjects
{
    class Shaders;
}

class ShaderCompiler
{
public:
    static bool process(const QJsonObject & config);
    
private:
    static QSurfaceFormat parseOpenGLFormat(
        const QJsonObject & config,
        bool & ok);
    
    static bool makeContextCurrent(
        QOpenGLContext & context,
        QOffscreenSurface & surface,
        const std::function<bool ()> & functor);
    
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
    
    static std::vector<globjects::ref_ptr<globjects::Shaders>> parseShaders(
        const QJsonArray & shaders,
        bool & ok);
    
    static bool createAndLinkProgram(
        const std::vector<globjects::ref_ptr<globjects::Shaders>> & shadersArray);
};
