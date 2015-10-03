#include "ShaderCompiler.h"

#include <cassert>

#include <vector>

#include <QJsonArray>
#include <QJsonObject>

#include <iozeug/filename.h>
#include <iozeug/directorytraversal.h>

#include <glbinding/gl/functions.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/NamedString.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/logging.h>

#include "OpenGLContext.h"


bool ShaderCompiler::process(const QJsonObject & config)
{
    return ShaderCompiler{}.parse(config);
}

bool ShaderCompiler::parse(const QJsonObject & config)
{
    bool ok{};
    
    const auto jsonOpenGLConfig = config.value("opengl");
    
    if (!jsonOpenGLConfig.isObject())
        return false;
    
    auto context = OpenGLContext::fromJsonConfig(jsonOpenGLConfig.toObject(), &ok);
    
    if (!ok)
        return false;
    
    if (!context.create())
        return false;
    
    if (!context.makeCurrent())
        return false;
    
    globjects::init();
    
    printDriverInfo();
    
    const auto jsonNamedStringPaths = config.value("namedStringPaths");
    
    if (jsonNamedStringPaths.isArray())
    {
        parseNamedStringPaths(jsonNamedStringPaths.toArray());
    }
    
    const auto jsonPrograms = config.value("programs");
    
    if (!jsonPrograms.isArray())
        return false;
    
    ok = parsePrograms(jsonPrograms.toArray());
    
    context.doneCurrent();
    
    printFailures();
    
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
        
        globjects::info();
        globjects::info() << "Process " << name.toStdString();
        
        const auto shadersArray = programObject.value("shaders");
        
        if (!shadersArray.isArray())
            return false;
        
        const auto shaders = parseShaders(shadersArray.toArray(), ok);
        
        if (!ok)
            continue;
        
        globjects::info() << "Link " << name.toStdString();
        
        ok = createAndLinkProgram(shaders);
        
        if (!ok)
            m_linkFailures.push_back(name.toStdString());
    }
    
    return true;
}

std::vector<globjects::ref_ptr<globjects::Shader>> ShaderCompiler::parseShaders(
    const QJsonArray & shadersArray,
    bool & ok)
{
    std::vector<globjects::ref_ptr<globjects::Shader>> shaders{};
    
    for (const auto & shaderValue : shadersArray)
    {
        if (!shaderValue.isObject())
        {
            ok = false;
            return shaders;
        }
        
        const auto shaderObject = shaderValue.toObject();

        const auto fileName = shaderObject.value("file").toString();

        if (fileName.isNull())
        {
            ok = false;
            return shaders;
        }

        const auto type = typeFromString(shaderObject.value("type").toString());

        if (type == gl::GL_NONE)
        {
            ok = false;
            return shaders;
        }

        // TODO: parse optional replacements

        auto shader = globjects::Shader::fromFile(type, fileName.toStdString());

        const auto name = shaderObject.value("name").toString();
        
        globjects::info() << "Compile " << (!name.isNull() ? name.toStdString() : fileName.toStdString());
        
        if (!shader->compile())
        {
            m_compileFailures.push_back(fileName.toStdString());
            
            ok = false;
            return shaders;
        }

        shaders.push_back(globjects::ref_ptr<globjects::Shader>(shader));
    }
    
    ok = true;
    return shaders;
}

gl::GLenum ShaderCompiler::typeFromString(const QString & typeString)
{
    if (typeString == "GL_VERTEX_SHADER")
    {
        return gl::GL_VERTEX_SHADER;
    }
    else if (typeString == "GL_VERTEX_SHADER")
    {
        return gl::GL_TESS_CONTROL_SHADER;
    }
    else if (typeString == "GL_TESS_EVALUATION_SHADER")
    {
        return gl::GL_TESS_EVALUATION_SHADER;
    }
    else if (typeString == "GL_GEOMETRY_SHADER")
    {
        return gl::GL_GEOMETRY_SHADER;
    }
    else if (typeString == "GL_FRAGMENT_SHADER")
    {
        return gl::GL_FRAGMENT_SHADER;
    }
    else if (typeString == "GL_COMPUTE_SHADER")
    {
        return gl::GL_COMPUTE_SHADER;
    }

    return gl::GL_NONE;
}

bool ShaderCompiler::createAndLinkProgram(
    const std::vector<globjects::ref_ptr<globjects::Shader>> & shaders)
{
    auto program = globjects::make_ref<globjects::Program>();

    for (auto & shader : shaders)
        program->attach(shader);

    program->link();

    if (!program->isLinked())
        return false;
    
    return true;
}

void ShaderCompiler::printDriverInfo()
{
    globjects::info() << "Driver: " << globjects::vendor();
    globjects::info() << "Renderer: " << globjects::renderer();
}

void ShaderCompiler::printFailures()
{
    if (!m_compileFailures.empty())
    {
        globjects::info();
        globjects::info() << "Compile Failures:";
        for (const auto failure : m_compileFailures)
            globjects::info() << "    " << failure;
    }
    
    if (!m_linkFailures.empty())
    {
        globjects::info();
        globjects::info() << "Link Failures:";
        for (const auto failure : m_linkFailures)
            globjects::info() << "    " << failure;
    }
}