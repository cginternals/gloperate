#include "ShaderCompiler.h"

#include <cassert>

#include <iostream>
#include <vector>
#include <algorithm>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileInfo>

#include <glbinding/gl/enum.h>

#include <iozeug/filename.h>
#include <iozeug/directorytraversal.h>

#include <glbinding/gl/functions.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/NamedString.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/logging.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>

#include "OpenGLContext.h"
#include "JsonParseErrorLog.h"

ShaderCompiler::ShaderCompiler(const QString & basePath)
: m_basePath(basePath)
{
}

bool ShaderCompiler::process(const QString & basePath, const QJsonDocument & configDocument)
{
    return ShaderCompiler(basePath).parse(configDocument);
}

bool ShaderCompiler::parse(const QJsonDocument & configDocument)
{
    if (configDocument.isObject())
    {
        return parse(configDocument.object());
    }
    else if (configDocument.isArray())
    {
        return parse(configDocument.array());
    }
    else
    {
        error(JsonParseError::DocumentNotAnObjectOrArray);

        return false;
    }
}

bool ShaderCompiler::parse(const QJsonArray & config)
{
    for (const auto & value : config)
    {
        if (!value.isObject())
        {
            error(JsonParseError::ElementNotObject);

            return false;
        }

        if (!parse(value.toObject()))
        {
            return false;
        }
    }

    return true;
}

bool ShaderCompiler::parse(const QJsonObject & config)
{
    const auto jsonOpenGLConfig = config.value("opengl");

    if (!jsonOpenGLConfig.isObject())
    {
        error(JsonParseError::PropertyNotFoundOrNotAnObject, "opengl");
        return false;
    }

    auto parseError = JsonParseError{};
    auto context = OpenGLContext::fromJsonConfig(jsonOpenGLConfig.toObject(), &parseError);

    if (parseError)
    {
        error(parseError);
        return false;
    }

    if (!context.create())
    {
        error(JsonParseError::ContextCreationFailed);
        return false;
    }

    if (!context.makeCurrent())
    {
        error(JsonParseError::ContextActivationFailed);
        return false;
    }

    globjects::init();

    info(Info::Driver);

    const auto jsonNamedStringPaths = config.value("namedStringPaths");

    if (jsonNamedStringPaths.isArray())
    {
        if (!parseNamedStringPaths(jsonNamedStringPaths.toArray()))
            return false;
    }

    const auto jsonPrograms = config.value("programs");

    if (!jsonPrograms.isArray())
    {
        error(JsonParseError::ArrayNotFoundOrEmpty, "programs");
        return false;
    }

    auto ok = parsePrograms(jsonPrograms.toArray());

    context.doneCurrent();

    info(Info::Failures);

    return ok;
}

bool ShaderCompiler::parseNamedStringPaths(const QJsonArray & paths)
{
    auto namedStrings = std::vector<std::string>();
    
    for (const auto & namedStringPath : paths)
    {
        if (!namedStringPath.isObject())
        {
            error(JsonParseError::ElementNotObject, "namedStringPaths");
            return false;
        }
        
        const auto pathObject = namedStringPath.toObject();
        
        const auto pathString = pathObject.value("path").toString();
        
        if (pathString.isNull())
        {
            error(JsonParseError::PropertyNotFoundOrWrongFormat, "path");
            return false;
        }
        
        const auto extensionsArray = pathObject.value("extensions").toArray();
        
        if (extensionsArray.isEmpty())
        {   
            error(JsonParseError::ArrayNotFoundOrEmpty, "extensions" );
            return false;
        }
        
        auto ok = false;
        const auto extensions = parseExtensions(extensionsArray, ok);
        
        if (!ok)
        {
            error(JsonParseError::ElementWrongFormat, "extensions");
            return false;
        }
        
        auto files = scanDirectory(pathString.toStdString(), extensions);
        
        if (files.empty())
        {
            error(JsonParseError::NoFilesWithExtensionFound, pathString);
            return false;
        }
        
        const auto aliasString = pathObject.value("alias").toString();
        
        auto aliases = files;
        
        if (!aliasString.isNull())
        {
            aliases = createAliases(files,
                pathString.toStdString(),
                aliasString.toStdString());
        }
        
        std::copy(aliases.begin(), aliases.end(), std::back_inserter(namedStrings));
        
        createNamedStrings(files, aliases);
    }
    
    qDebug() << "Registered Named Strings:";
    for (const auto & namedString : namedStrings)
        qDebug().nospace() << "    " << QString::fromStdString(namedString);

    return true;
}

std::set<std::string> ShaderCompiler::parseExtensions(
    const QJsonArray & extensionsArray,
    bool & ok)
{
    auto extensions = std::set<std::string>();
    
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
    auto files = std::vector<std::string>();
    
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
    auto aliases = std::vector<std::string>();

    for (const auto & file : files)
    {
        auto aliasedFile = alias;
        
        assert(file.size() >= path.size());
        std::copy(file.begin() + path.size(), file.end(), std::back_inserter(aliasedFile));
        
        aliases.push_back(aliasedFile);
    }

    return aliases;
}

void ShaderCompiler::createNamedStrings(
    const std::vector<std::string> & files,
    const std::vector<std::string> & aliases)
{
    assert(files.size() == aliases.size());
    
    for (auto i = size_t(0); i < files.size(); ++i)
    {
        const auto & file = files[i];
        const auto & alias = aliases[i];

        globjects::NamedString::create(alias, new globjects::File(file));
    }
}

bool ShaderCompiler::parsePrograms(const QJsonArray & programs)
{
    for (const auto programValue : programs)
    {
        if (!programValue.isObject())
        {
            error(JsonParseError::ElementNotObject, "programs");
            return false;
        }
        
        const auto programObject = programValue.toObject();
        
        const auto name = programObject.value("name").toString();
        
        if (name.isNull())
        {
            error(JsonParseError::PropertyNotFoundOrWrongFormat, "name");
            return false;
        }
        
        qDebug() << "";
        qDebug().noquote() << "Process" << name;
        
        const auto shadersArray = programObject.value("shaders");
        
        if (!shadersArray.isArray())
        {
            error(JsonParseError::ArrayNotFoundOrEmpty, "shaders");
            return false;
        }
        
        auto ok = false;
        const auto shaders = parseShaders(shadersArray.toArray(), ok);
        
        if (!ok)
        {
            m_linkFailures.push_back(name.toStdString());
            continue;
        }
        
        qDebug().noquote() << "Link" << name;
        
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
    auto shaders = std::vector<globjects::ref_ptr<globjects::Shader>>();
    
    for (const auto & shaderValue : shadersArray)
    {
        if (!shaderValue.isObject())
        {
            error(JsonParseError::ElementNotObject, "shaders");
            ok = false;
            return shaders;
        }
        
        const auto shaderObject = shaderValue.toObject();

        auto fileName = shaderObject.value("file").toString();

        if (fileName.isNull())
        {
            error(JsonParseError::PropertyNotFoundOrWrongFormat, "file");
            ok = false;
            return shaders;
        }
        
        const auto name = shaderObject.value("name").toString();
        
        if (name.isNull())
            qDebug().noquote() << QString{"Compile %1"}.arg(fileName);
        else
            qDebug().noquote() << QString{"Compile %1 ('%2')"}.arg(name).arg(fileName);

        const auto typeString = shaderObject.value("type").toString();
        
        if (typeString.isNull())
        {
            error(JsonParseError::PropertyNotFoundOrWrongFormat, "type");
            ok = false;
            return shaders;
        }
        
        const auto type = typeFromString(typeString);

        if (type == gl::GL_NONE)
        {
            error(JsonParseError::ShaderTypeNotFound, typeString);
            ok = false;
            return shaders;
        }

        QFileInfo info(fileName);
        if (info.isRelative())
        {
            fileName = m_basePath + "/" + fileName;
        }

        globjects::ref_ptr<globjects::AbstractStringSource> shaderFile = new globjects::File(fileName.toStdString());

        const auto replacementsValue = shaderObject.value("replacements");
        
        if (!replacementsValue.isUndefined())
        {
            if (replacementsValue.isObject())
            {
                if (!replaceStrings(replacementsValue.toObject(), shaderFile))
                {
                    ok = false;
                    return shaders;
                }
            }
            else
            {
                error(JsonParseError::PropertyWrongFormat, "replacements");
                ok = false;
                return shaders;
            }
        }
        
        auto shader = globjects::make_ref<globjects::Shader>(type, shaderFile);
        
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

bool ShaderCompiler::replaceStrings(
    const QJsonObject & replacements,
    globjects::ref_ptr<globjects::AbstractStringSource> & stringSource)
{
    auto sourceTemplate = globjects::make_ref<globjects::StringTemplate>(stringSource);
    
    for (auto it = replacements.begin(); it != replacements.end(); ++it)
    {
        const auto valueString = it.value().toString();
        
        if (valueString.isNull())
        {
            error(JsonParseError::PropertyWrongFormat, it.key());
            return false;
        }
        
        sourceTemplate->replace(it.key().toStdString(), valueString.toStdString());
    }
    
    stringSource = sourceTemplate;
    return true;
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

void ShaderCompiler::info(Info type)
{
    if (type == Info::Driver)
    {
        globjects::info() << "Driver: " << globjects::vendor();
        globjects::info() << "Renderer: " << globjects::renderer();
    }
    else if (type == Info::Failures)
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
}

void ShaderCompiler::error(JsonParseError error)
{
    JsonParseErrorLog::error(error);
}

void ShaderCompiler::error(JsonParseError::Type type, const QString & info)
{
    JsonParseErrorLog::error({ type, info });
}
