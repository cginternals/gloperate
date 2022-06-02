
#include <gloperate-headless/Surface.h>

#include <cassert>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <eglbinding/egl/egl.h>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/GLContext.h>
#include <gloperate-headless/GLContextFactory.h>
#include <gloperate-headless/Application.h>


using namespace egl;
using namespace gloperate;


namespace gloperate_headless
{


std::set<Surface*> Surface::s_instances;


const std::set<Surface*> & Surface::instances()
{
    return s_instances;
}


Surface::Surface(EGLDisplay display)
: m_title("")
, m_format()
, m_display(display)
, m_size(1920, 1080)
, m_quitOnDestroy(true)
, m_needsRepaint(false)
, m_context(nullptr)
{
    // Register surface
    s_instances.insert(this);
}

Surface::~Surface()
{
    // Unregister window
    s_instances.erase(this);

    // Destroy window
    destroy();
}

bool Surface::setContextFormat(const gloperate::GLContextFormat & format)
{
    // If window has already been created, the context format cannot be changed anymore
    if (m_context)
    {
        return false;
    }

    // Save new context format
    m_format = format;
    return true;
}

bool Surface::create()
{
    // Abort, if the window has already been created
    if (m_context)
    {
        return false;
    }

    // Create internal surface
    if (!createInternalSurface(m_format, m_size.x, m_size.y, false, m_display))
    {
        cppassist::critical() << "Creating native surface with OpenGL context failed.";
        return false;
    }

    // Success
    return true;
}

void Surface::destroy()
{
    // Destroy internal window
    if (m_context)
    {
        destroyInternalSurface();
    }

    // Quit application?
    if (m_quitOnDestroy)
    {
        Application::quit(0);
    }
}

const GLContext * Surface::context() const
{
    return m_context.get();
}

GLContext * Surface::context()
{
    return m_context.get();
}

bool Surface::recreateSurface()
{
    // Destroy internal surface
    destroyInternalSurface();

    // Create new internal surface
    return createInternalSurface(m_format, m_size.x, m_size.y, true, m_display);
}

const std::string & Surface::title() const
{
    return m_title;
}

void Surface::setTitle(const std::string & title)
{
    m_title = title;
}

bool Surface::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void Surface::setQuitOnDestroy(bool quitOnDestroy)
{
    m_quitOnDestroy = quitOnDestroy;
}

void Surface::repaint()
{
    if (!m_context)
    {
        return;
    }

    m_needsRepaint = true;

    Application::wakeup();
}

void Surface::swap()
{
    if (!m_display || !m_context)
    {
        return;
    }

    eglSwapBuffers(m_display, m_context->surface());
}

void Surface::idle()
{
    onIdle();
}

EGLSurface Surface::internalSurface() const
{
    return m_context->surface();
}

bool Surface::hasPendingEvents()
{
    return false;
}

void Surface::processEvents()
{
}

bool Surface::createInternalSurface(const GLContextFormat & format, int width, int height, bool setContextActive, EGLDisplay display)
{
    // Abort if surface is already created
    assert(!m_context);
    if (m_context)
    {
        cppassist::warning("gloperate-headless") << "Attempt to create EGL surface although a context is already initialized";

        return false;
    }

    // Create EGL surface with OpenGL context
    GLContextFactory factory(display, width, height);
    m_context.reset(static_cast<GLContext*>( factory.createBestContext(format).release() ));

    // Check if context has been created
    if (!m_context)
    {
        cppassist::warning("gloperate-headless") << "Attempt to create EGL surface but context couldn't get created";

        return false;
    }

    // Check OpenGL format
    m_context->format().verify(format);

    // Initialize rendering components with new context
    m_context->use();
    cppassist::debug("gloperate-headless") << "Initialize new context";
    onContextInit();

    if (!setContextActive)
    {
        m_context->release();
    }

    // Success
    return true;
}

void Surface::destroyInternalSurface()
{
    // Abort if window has not been created
    if (!m_context)
    {
        cppassist::warning("gloperate-headless") << "Deinitialize EGL surface although none is initialized";

        return;
    }

    // Deinitialize rendering components from old context
    m_context->use();
    cppassist::debug("gloperate-headless") << "Deinitialize EGL surface";
    onContextDeinit();
    m_context->release();

    // Destroy EGL context
    eglDestroyContext(m_display, m_context->context());
    eglDestroySurface(m_display, m_context->surface());

    // Reset internal pointers
    m_context = nullptr;

    cppassist::debug("gloperate-headless") << "EGL surface deinitialized";
}

void Surface::onContextInit()
{
}

void Surface::onContextDeinit()
{
}

void Surface::onIdle()
{
}


} // namespace gloperate_headless
