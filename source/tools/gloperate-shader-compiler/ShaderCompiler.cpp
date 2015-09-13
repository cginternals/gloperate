#include "ShaderCompiler.h"

#include <cassert>

#include <vector>

#include <QJsonArray>
#include <QJsonObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include <iozeug/filename.h>
#include <iozeug/directorytraversal.h>

#include <globjects/base/File.h>
#include <globjects/NamedString.h>


bool ShaderCompiler::process(const QJsonObject & config)
{
    bool ok{};
    
    const auto jsonOpenGLConfig = config.value("opengl");
    
    if (!jsonOpenGLConfig.isObject())
        return false;

    const auto format = parseOpenGLFormat(jsonOpenGLConfig.toObject(), ok);
    
    if (!ok)
        return false;

    QOpenGLContext context{};
    context.setFormat(format);
    
    if (!context.create())
        return false;
    
    QOffscreenSurface surface{};
    surface.setFormat(context.format());
    surface.create();
    
    ok = makeContextCurrent(context, surface,
        [&config] ()
        {
            const auto jsonNamedStringPaths = config.value("namedStringPaths");
            
            if (!jsonNamedStringPaths.isArray())
                return false;
            
            parseNamedStringPaths(jsonNamedStringPaths.toArray());
            
            const auto jsonPrograms = config.value("programs");
            
            if (!jsonPrograms.isArray())
                return false;
            
            return parsePrograms(jsonPrograms.toArray());;
        });
    
    return ok;
}

QSurfaceFormat ShaderCompiler::parseOpenGLFormat(const QJsonObject & config, bool & ok)
{
    static const auto hasWrongFormat = -1;

    QSurfaceFormat format{};
    format.setRenderableType(QSurfaceFormat::OpenGL);

    const auto majorVersion = config.value("major").toInt(hasWrongFormat);

    if (majorVersion == hasWrongFormat)
    {
        ok = false;
        return format;
    }

    const auto minorVersion = config.value("minor").toInt(hasWrongFormat);

    if (minorVersion == hasWrongFormat)
    {
        ok = false;
        return format;
    }

    format.setVersion(majorVersion, minorVersion);
    
    if (format.version() >= qMakePair(3, 2))
    {
        const auto jsonCoreFlag = config.value("core");
        const auto profile = jsonCoreFlag.toBool(true) ? QSurfaceFormat::CoreProfile :
            QSurfaceFormat::CompatibilityProfile;
        
        format.setProfile(profile);
    }
    
    if (format.version() >= qMakePair(3, 0))
    {
        const auto jsonForwardFlag = config.value("forward");
        
        if (jsonForwardFlag.toBool(false))
            format.setOption(QSurfaceFormat::DeprecatedFunctions);
    }
    
    const auto jsonDebugFlag = config.value("debug");
    
    if (jsonDebugFlag.toBool(false))
        format.setOption(QSurfaceFormat::DebugContext);

    ok = true;
    return format;
}

bool ShaderCompiler::makeContextCurrent(
    QOpenGLContext & context,
    QOffscreenSurface & surface,
    const std::function<bool ()> & functor)
{
    context.makeCurrent(&surface);
    const auto ok = functor();
    context.doneCurrent();
    
    return ok;
}

void ShaderCompiler::parseNamedStringPaths(const QJsonArray & paths)
{
    bool ok{};
    
    for (const auto & namedStringPath : paths)
    {
        if (!namedStringPath.isObject())
            continue;
        
        const auto pathObject = namedStringPath.toObject();
        
        const auto pathString = pathObject.value("path").toString();
        
        if (pathString.isNull())
            continue;
        
        const auto extensionsArray = pathObject.value("extensions").toArray();
        
        if (extensionsArray.isEmpty())
            continue;
        
        const auto extensions = parseExtensions(extensionsArray, ok);
        
        if (!ok)
            continue;
        
        auto files = scanDirectory(pathString.toStdString(), extensions);
        
        const auto aliasString = pathObject.value("alias").toString();
        
        if (aliasString.isNull())
        {
            createNamedStrings(files, files);
        }
        else
        {
            const auto aliases = createAliases(files,
                pathString.toStdString(),
                aliasString.toStdString());
            
            createNamedStrings(files, aliases);
        }
    }
}

std::set<std::string> ShaderCompiler::parseExtensions(
    const QJsonArray & extensionsArray,
    bool & ok)
{
    auto extensions = std::set<std::string>{};
    
    for (const auto & extensionValue : extensionsArray)
    {
        if (!extensionValue.isString())
        {
            ok = false;
            return extensions;
        }
        
        extensions.insert(extensionValue.toString().toStdString());
    }
    
    ok = true;
    return extensions;
}

std::vector<std::string> ShaderCompiler::scanDirectory(
    const std::string & path,
    const std::set<std::string> & extensions)
{
    auto files = std::vector<std::string>{};
    
    iozeug::scanDirectory(path, "*", true,
        [&extensions, &files] (const std::string & fileName)
        {
          const auto fileExtension = iozeug::getExtension(fileName);
          
          if (!extensions.count(fileExtension))
              return;
          
          files.push_back(fileName);
        });

    return files;
}

std::vector<std::string> ShaderCompiler::createAliases(
    const std::vector<std::string> & files,
    const std::string & path,
    const std::string & alias)
{
    // TODO: replace path with alias
    return files;
}

void ShaderCompiler::createNamedStrings(
    const std::vector<std::string> & files,
    const std::vector<std::string> & aliases)
{
    assert(files.size() == aliases.size());
    
    for (auto i = 0u; i < files.size(); ++i)
    {
        const auto file = files[i];
        const auto alias = aliases[i];
        
        const auto fileObject = new globjects::File(file);
        
        globjects::NamedString::create(alias, fileObject);
    }
}

bool ShaderCompiler::parsePrograms(const QJsonArray & programs)
{
    bool ok{};

    for (const auto programValue : programs)
    {
        if (!programValue.isObject())
            return false;
        
        const auto programObject = programValue.toObject();
        
        const auto name = programObject.value("name").toString();
        
        if (name.isNull())
            return false;
        
        const auto shadersArray = programObject.value("shaders");
        
        if (!shadersArray.isArray())
            return false;
        
        const auto shaders = parseShaders(shadersArray.toArray(), ok);
        
        if (!ok)
            return false;
        
        ok = createAndLinkProgram(shaders);
        
        if (!ok)
            return false;
    }
    
    return true;
}

std::vector<globjects::ref_ptr<globjects::Shaders>> ShaderCompiler::parseShaders(
    const QJsonArray & shadersArray,
    bool & ok)
{
    std::vector<globjects::ref_ptr<globjects::Shaders>> shaders{};
    
    for (const auto & shaderValue : shadersArray)
    {
        if (!shaderValue.isObject())
        {
            ok = false;
            return shaders;
        }
        
        // TODO: continue
    }
    
    ok = true;
    return shaders;
}

bool ShaderCompiler::createAndLinkProgram(
    const std::vector<globjects::ref_ptr<globjects::Shaders>> & shaders)
{
    // TODO: implement
    return false;
}



