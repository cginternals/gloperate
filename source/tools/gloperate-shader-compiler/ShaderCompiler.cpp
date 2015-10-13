#include "ShaderCompiler.h"

#include <set>

#include <QStringList>
#include <QFileInfo>
#include <QDebug>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include <iozeug/filename.h>
#include <iozeug/directorytraversal.h>

#include <gloperate/base/registernamedstrings.h>

namespace
{

gl::GLenum convertToType(const std::string & type)
{
    if (type == "vert")
    {
        return gl::GL_VERTEX_SHADER;
    }
    else if (type == "tcs")
    {
        return gl::GL_TESS_CONTROL_SHADER;
    }
    else if (type == "tes")
    {
        return gl::GL_TESS_EVALUATION_SHADER;
    }
    else if (type == "geom")
    {
        return gl::GL_GEOMETRY_SHADER;
    }
    else if (type == "frag")
    {
        return gl::GL_FRAGMENT_SHADER;
    }
    else if (type == "comp")
    {
        return gl::GL_COMPUTE_SHADER;
    }

    return gl::GL_NONE;
}

}

ShaderCompiler::ShaderCompiler(const QList<QString> & includePathsAndAliases)
{
    globjects::init();

    for (const QString & path : includePathsAndAliases)
    {
        if (path.contains(':'))
        {
            auto list = path.split(':', QString::KeepEmptyParts);

            gloperate::registerNamedStrings(list[0].toStdString(), list[1].toStdString(), "glsl", true);
        }
        else
        {
            gloperate::registerNamedStrings(path.toStdString(), "glsl", true);
        }
    }
}

bool ShaderCompiler::compile(const QString & shaderFile)
{
    QFileInfo info(shaderFile);

    if (!info.exists())
    {
        qDebug() << "File not found";

        return false;
    }

    if (info.isFile())
    {
        return compileFile(shaderFile.toStdString(), iozeug::getExtension(shaderFile.toStdString()));
    }

    assert(info.isDir());

    auto success = true;
    const auto extensions = std::set<std::string>{ "vert", "tcs", "tes", "geom", "frag", "comp" };
    iozeug::scanDirectory(shaderFile.toStdString(), "*", true, [this, & extensions, & success](const std::string & file) {
        const auto extension = iozeug::getExtension(file);

        if (extensions.count(extension) > 0)
        {
            success &= compileFile(file, extension);
        }
    });

    return success;
}

bool ShaderCompiler::compileFile(const std::string & shaderFile, const std::string & extension)
{
    qDebug() << "Compile" << shaderFile.c_str();

    auto shaderType = convertToType(extension);

    if (shaderType == gl::GL_NONE)
    {
        globjects::debug() << "Shader type not detected";

        return false;
    }

    globjects::ref_ptr<globjects::Shader> shader = globjects::Shader::fromFile(shaderType, shaderFile);
    return shader->compile();
}
