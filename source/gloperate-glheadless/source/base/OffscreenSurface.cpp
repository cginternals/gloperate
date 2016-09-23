
#include "gloperate-glheadless/base/OffscreenSurface.h"

#include <glheadless/Context.h>

#include <gloperate-glheadless/base/GLContext.h>
#include <gloperate-glheadless/base/GLContextFactory.h>


namespace gloperate_headless
{


OffscreenSurface::OffscreenSurface()
: m_initialized(false)
{
}


OffscreenSurface::~OffscreenSurface()
{
}


void OffscreenSurface::setContextFormat(const gloperate::GLContextFormat& format)
{
    m_format = format;
}


void OffscreenSurface::createContext()
{
    // Destroy old context
    if (m_context)
    {
        deinitializeContext();
        m_context = nullptr;
    }

    // Create OpenGL context
    GLContextFactory factory;
    m_context.reset(static_cast<GLContext*>(factory.createBestContext(m_format)));

    // Initialize new context
    initializeContext();
}


void OffscreenSurface::destroyContext()
{
    // Destroy old context
    if (!m_context)
    {
        return;
    }

    deinitializeContext();

    m_context = nullptr;
}


GLContext* OffscreenSurface::context() const
{
    return m_context.get();
}


void OffscreenSurface::updateGL()
{
    paint();
}


void OffscreenSurface::initializeContext()
{
    if (m_initialized)
    {
        return;
    }

    m_context->glheadlessContext()->makeCurrent();
    onContextInit();
    m_context->glheadlessContext()->doneCurrent();

    m_initialized = true;
}


void OffscreenSurface::deinitializeContext()
{
    if (!m_initialized)
    {
        return;
    }

    m_context->glheadlessContext()->makeCurrent();
    onContextDeinit();
    m_context->glheadlessContext()->doneCurrent();

    m_initialized = false;
}


void OffscreenSurface::resize(int width, int height)
{
    if (!m_initialized)
    {
        initializeContext();
    }

    m_context->glheadlessContext()->makeCurrent();

    onResize(width, height);

    m_context->glheadlessContext()->doneCurrent();
}


void OffscreenSurface::paint()
{
    if (!m_initialized)
    {
        initializeContext();
    }

    m_context->glheadlessContext()->makeCurrent();

    onPaint();

    m_context->glheadlessContext()->doneCurrent();
}


void OffscreenSurface::onContextInit()
{
}


void OffscreenSurface::onContextDeinit()
{
}


void OffscreenSurface::onResize(int /*width*/, int /*height*/)
{
}


void OffscreenSurface::onPaint()
{
}


} // namespace gloperate_headless
