#include "gloperate-glheadless/base/GLContextFactory.h"

#include <cppassist/logging/logging.h>

#include <glheadless/Context.h>
#include <glheadless/ContextFormat.h>
#include <glheadless/ContextFactory.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <gloperate-glheadless/base/GLContext.h>


namespace gloperate_glheadless
{


glheadless::ContextFormat GLContextFactory::toGlhContextFormat(const gloperate::GLContextFormat & format)
{
    glheadless::ContextFormat glhFormat;
    glhFormat.versionMajor = format.majorVersion();
    glhFormat.versionMinor = format.minorVersion();
    glhFormat.debug = format.debugContext();
    switch (format.profile()) {
        case gloperate::GLContextFormat::Profile::Core:
            glhFormat.profile = glheadless::ContextProfile::CORE;
            break;
        case gloperate::GLContextFormat::Profile::Compatibility:
            glhFormat.profile = glheadless::ContextProfile::COMPATIBILITY;
            break;
        default:
            break;
    }

    return glhFormat;
}


GLContextFactory::GLContextFactory()
: m_sharedContext(nullptr)
, m_shareCurrent(false)
{
}


GLContextFactory::~GLContextFactory()
{
}


gloperate::AbstractContext * GLContextFactory::sharedContext() const
{
    return m_sharedContext;
}


void GLContextFactory::setSharedContext(gloperate::AbstractContext * sharedContext)
{
    m_sharedContext = sharedContext;
}


bool GLContextFactory::shareCurrent() const
{
    return m_shareCurrent;
}


void GLContextFactory::setShareCurrent(bool shareCurrent)
{
    m_shareCurrent = shareCurrent;
}


gloperate::AbstractGLContext * GLContextFactory::createContext(const gloperate::GLContextFormat & format)
{
    auto currentGlhContext = glheadless::ContextFactory::getCurrent();
    if (currentGlhContext->valid())
    {
        currentGlhContext->doneCurrent();
    }
    
    std::unique_ptr<glheadless::Context> glhContext;

    if (m_sharedContext != nullptr)
    {
        m_sharedContext->use();
        auto sharedGlhContext = glheadless::ContextFactory::getCurrent();
        m_sharedContext->release();

        if (sharedGlhContext->valid())
        {
            glhContext = createGlhContext(sharedGlhContext.get(), format);
        }
        else
        {
            cppassist::warning() << "Could not retrieve shared context handle: " << sharedGlhContext->lastErrorMessage();
        }
    }
    else if (m_shareCurrent)
    {
        if (currentGlhContext->valid())
        {
            glhContext = createGlhContext(currentGlhContext.get(), format);
        }
        else
        {
            cppassist::warning() << "Could not retrieve current context handle: " << currentGlhContext->lastErrorMessage();
        }
    }
    else
    {
        glhContext = createGlhContext(nullptr, format);
    }


    if (currentGlhContext->valid())
    {
        currentGlhContext->makeCurrent();
    }

    if (glhContext)
    {
        return new GLContext(std::move(glhContext));
    }
    return nullptr;
}


std::unique_ptr<glheadless::Context> GLContextFactory::createGlhContext(glheadless::Context * sharedGlhContext, const gloperate::GLContextFormat & format)
{
    auto glhFormat = toGlhContextFormat(format);

    auto glhContext = sharedGlhContext != nullptr ?
        glheadless::ContextFactory::create(sharedGlhContext, glhFormat) :
        glheadless::ContextFactory::create(glhFormat);

    if (!glhContext->valid())
    {
        cppassist::warning() << "Could not create OpenGL context: " << glhContext->lastErrorMessage();
        return nullptr;
    }

    glhContext->makeCurrent();

    glbinding::Binding::initialize(false);

    const auto versionString = gl::glGetString(gl::GL_VERSION);
    cppassist::info() << "Created OpenGL Context " << reinterpret_cast<const char*>(versionString);

    glhContext->doneCurrent();

    return glhContext;
}


} // namespace gloperate_glheadless
