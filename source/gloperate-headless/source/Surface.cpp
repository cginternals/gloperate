
#include <gloperate-headless/Surface.h>

#include <cassert>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <eglbinding/egl/egl.h>
#undef KeyPress
#undef KeyRelease

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/GLContext.h>
#include <gloperate-headless/GLContextFactory.h>
#include <gloperate-headless/Application.h>
#include <gloperate-headless/SurfaceEvent.h>


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
    // Unregister surface
    s_instances.erase(this);

    // Destroy surface
    destroy();
}

bool Surface::setContextFormat(const gloperate::GLContextFormat & format)
{
    // If surface has already been created, the context format cannot be changed anymore
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
    // Abort, if the surface has already been created
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
    // Destroy internal surface
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

void Surface::dispose()
{
    if (!m_context)
    {
        return;
    }

    queueEvent(cppassist::make_unique<CloseEvent>());
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

glm::ivec2 Surface::size() const
{
    return glm::ivec2(m_size.x, m_size.y);
}

void Surface::setSize(int width, int height)
{
    if (m_size.x == width || m_size.y == height)
    {
        return;
    }

    m_size.x = width;
    m_size.y = height;

    if (m_context != nullptr)
    {
        recreateSurface();
    }
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

void Surface::queueEvent(std::unique_ptr<SurfaceEvent> && event)
{
    if (!event)
    {
        return;
    }

    m_eventQueue.push(std::move(event));
}

void Surface::updateRepaintEvent()
{
    if (m_needsRepaint)
    {
        m_needsRepaint = false;

        queueEvent(cppassist::make_unique<PaintEvent>());
    }
}

bool Surface::hasPendingEvents()
{
    if (!m_context)
    {
        return false;
    }

    return !m_eventQueue.empty();
}

void Surface::processEvents()
{
    if (m_eventQueue.empty() || !m_context)
    {
        return;
    }

    m_context->use();

    bool hasPaintEvent = false;

    while (!m_eventQueue.empty())
    {
        m_eventQueue.front()->setSurface(this);

        auto event = *m_eventQueue.front();

        if (event.type() == SurfaceEvent::Type::Paint) {
            // Postpone paint event until the end
            hasPaintEvent = true;
        } else {
            // Handle all other events immediately
            handleEvent(*m_eventQueue.front());
        }

        m_eventQueue.pop();

        // Handle destroy / dispose / quit events
        if (!m_context)
        {
            clearEventQueue();
            return;
        }
    }

    // Handle postponed paint event
    if (hasPaintEvent)
    {
        PaintEvent event;
        event.setSurface(this);

        handleEvent(event);
    }

    m_context->release();
}

void Surface::handleEvent(SurfaceEvent & event)
{
    // Check that event is meant for this surface
    if (event.surface() != this)
    {
        return;
    }

    // Dispatch event
    switch (event.type())
    {
        case SurfaceEvent::Type::Resize:
            onResize(static_cast<ResizeEvent &>(event));
            break;

        case SurfaceEvent::Type::Paint:
            onPaint(static_cast<PaintEvent &>(event));
            swap();
            break;

        case SurfaceEvent::Type::KeyPress:
            onKeyPress(static_cast<KeyEvent &>(event));
            break;

        case SurfaceEvent::Type::KeyRelease:
            onKeyRelease(static_cast<KeyEvent &>(event));
            break;

        case SurfaceEvent::Type::MousePress:
            onMousePress(static_cast<MouseEvent &>(event));
            break;

        case SurfaceEvent::Type::MouseRelease:
            onMouseRelease(static_cast<MouseEvent &>(event));
            break;

        case SurfaceEvent::Type::MouseMove:
            onMouseMove(static_cast<MouseEvent &>(event));
            break;

        case SurfaceEvent::Type::Scroll:
            onScroll(static_cast<MouseEvent &>(event));
            break;

        case SurfaceEvent::Type::Close:
            onClose(static_cast<CloseEvent &>(event));

            if (!event.isAccepted())
            {
                destroy();
            }

            break;

        default:
            break;
    }
}

void Surface::clearEventQueue()
{
    std::queue<std::unique_ptr<SurfaceEvent>> empty;
    std::swap(m_eventQueue, empty);
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

    // Promote current size
    queueEvent(cppassist::make_unique<ResizeEvent>(glm::ivec2(width, height)));

    // Success
    return true;
}

void Surface::destroyInternalSurface()
{
    // Abort if surface has not been created
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

void Surface::onResize(ResizeEvent &)
{
}

void Surface::onPaint(PaintEvent &)
{
}

void Surface::onKeyPress(KeyEvent &)
{
}

void Surface::onKeyRelease(KeyEvent &)
{
}

void Surface::onMousePress(MouseEvent &)
{
}

void Surface::onMouseMove(MouseEvent &)
{
}

void Surface::onMouseRelease(MouseEvent &)
{
}

void Surface::onScroll(MouseEvent &)
{
}

void Surface::onClose(CloseEvent &)
{
}

void Surface::onIdle()
{
}


} // namespace gloperate_headless
