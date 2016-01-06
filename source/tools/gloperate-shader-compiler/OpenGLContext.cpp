#include "OpenGLContext.h"

#include <QJsonObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QStringList>
#include <QDebug>

#include <gloperate/base/make_unique.hpp>

#include "JsonParseError.h"


class OpenGLContext::Private
{
public:
    void setFormat(const QSurfaceFormat & format);

    bool create();

    bool makeCurrent();
    void doneCurrent();

private:
    void printInfo();

private:
    std::unique_ptr<QOpenGLContext> m_context;
    std::unique_ptr<QOffscreenSurface> m_surface;
    QSurfaceFormat m_format;
};

void OpenGLContext::Private::setFormat(const QSurfaceFormat & format)
{
    m_format = format;
}

bool OpenGLContext::Private::create()
{
    m_context = gloperate::make_unique<QOpenGLContext>();
    m_context->setFormat(m_format);
    
    if (!m_context->create())
        return false;
    
    m_surface = gloperate::make_unique<QOffscreenSurface>();
    m_surface->setFormat(m_context->format());
    m_surface->create();

    if (!m_surface->isValid())
        return false;
    
    printInfo();

    return true;
}

bool OpenGLContext::Private::makeCurrent()
{
    return m_context->makeCurrent(m_surface.get());
}

void OpenGLContext::Private::doneCurrent()
{
    m_context->doneCurrent();
}

void OpenGLContext::Private::printInfo()
{
    const auto format = m_context->format();
    const auto version = format.version();
    
    auto options = QStringList{};
    
    if (version >= qMakePair(3, 2))
        options << (format.profile() == QSurfaceFormat::CoreProfile ? "Core" : "Compatibility");
    
    if (version >= qMakePair(3, 0))
    {
        if (format.testOption(QSurfaceFormat::DeprecatedFunctions))
            options << "forward compatibility";
    }
    
    if (format.testOption(QSurfaceFormat::DebugContext))
        options << "debug";
    
    qDebug().nospace().noquote() << "OpenGL Version: "
        << version.first << "." << version.second << " "
        << "(" << options.join(", ") << ")";
}

OpenGLContext OpenGLContext::fromJsonConfig(const QJsonObject & config, JsonParseError * error)
{
    static const auto hasWrongFormat = -1;

    QSurfaceFormat format{};
    format.setRenderableType(QSurfaceFormat::OpenGL);

    const auto majorVersion = config.value("major").toInt(hasWrongFormat);

    if (majorVersion == hasWrongFormat)
    {
        if (error)
            *error = { JsonParseError::PropertyNotFoundOrWrongFormat, "major" };

        return OpenGLContext{};
    }

    const auto minorVersion = config.value("minor").toInt(hasWrongFormat);

    if (minorVersion == hasWrongFormat)
    {
        if (error)
            *error = { JsonParseError::PropertyNotFoundOrWrongFormat, "minor" };

        return OpenGLContext{};
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

    auto context = OpenGLContext{};
    context.setFormat(format);

    if (error)
        *error = JsonParseError::NoError;
    
    return context;
}

OpenGLContext::OpenGLContext()
: m_p(gloperate::make_unique<Private>())
{
}

OpenGLContext::~OpenGLContext()
{
}

OpenGLContext::OpenGLContext(OpenGLContext && other)
: m_p(std::move(other.m_p))
{
}

OpenGLContext & OpenGLContext::operator=(OpenGLContext && other)
{
    m_p = std::move(other.m_p);

    return *this;
}

void OpenGLContext::setFormat(const QSurfaceFormat & format)
{
    m_p->setFormat(format);
}

bool OpenGLContext::create()
{
    return m_p->create();
}

bool OpenGLContext::makeCurrent()
{
    return m_p->makeCurrent();
}

void OpenGLContext::doneCurrent()
{
    m_p->doneCurrent();
}
