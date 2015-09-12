#include "ShaderCompiler.h"

#include <QJsonObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>



bool ShaderCompiler::process(const QJsonObject & config)
{
    const auto jsonOpenGLConfig = config.value("opengl");
    
    if (!jsonOpenGLConfig.isObject())
        return false;
    
    bool ok{};
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
    
    context.makeCurrent(&surface);
    
    // TODO: Parse Named Strings and Programs
    
    context.doneCurrent();
    
    return true;
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


