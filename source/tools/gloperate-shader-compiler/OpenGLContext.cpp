#include "OpenGLContext.h"

#include <QJsonObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include <gloperate/base/make_unique.hpp>


class OpenGLContext::Private
{
public:
    void setFormat(const QSurfaceFormat & format);

    bool create();

    void makeCurrent();
    void doneCurrent();

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

    return true;
}

void OpenGLContext::Private::makeCurrent()
{
    m_context->makeCurrent(m_surface.get());
}

void OpenGLContext::Private::doneCurrent()
{
    m_context->doneCurrent();
}

OpenGLContext OpenGLContext::fromJsonConfig(const QJsonObject & config, bool * ok)
{
    static const auto hasWrongFormat = -1;

    QSurfaceFormat format{};
    format.setRenderableType(QSurfaceFormat::OpenGL);

    const auto majorVersion = config.value("major").toInt(hasWrongFormat);

    if (majorVersion == hasWrongFormat)
    {
        if (ok)
            *ok = false;

        return OpenGLContext{};
    }

    const auto minorVersion = config.value("minor").toInt(hasWrongFormat);

    if (minorVersion == hasWrongFormat)
    {
        if (ok)
            *ok = false;

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

    if (ok)
        *ok = true;
    
    return context;
}

OpenGLContext::OpenGLContext()
:   m_p(gloperate::make_unique<Private>())
{
}

OpenGLContext::~OpenGLContext() = default;

OpenGLContext::OpenGLContext(OpenGLContext && context) = default;
OpenGLContext & OpenGLContext::operator=(OpenGLContext && rhs) = default;

void OpenGLContext::setFormat(const QSurfaceFormat & format)
{
    m_p->setFormat(format);
}

bool OpenGLContext::create()
{
    return m_p->create();
}

void OpenGLContext::makeCurrent()
{
    m_p->makeCurrent();
}

void OpenGLContext::doneCurrent()
{
    m_p->doneCurrent();
}
