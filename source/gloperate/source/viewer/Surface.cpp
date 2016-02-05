
#include <gloperate/viewer/Surface.h>


namespace gloperate
{


Surface::Surface(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_openGLContext(nullptr)
{
}

Surface::~Surface()
{
}

ViewerContext * Surface::viewerContext() const
{
    return m_viewerContext;
}

OpenGLContext * Surface::openGLContext() const
{
    return m_openGLContext;
}

void Surface::setOpenGLContext(OpenGLContext * context)
{
    // Deinitialize renderer in old context
    if (m_openGLContext)
    {
        onContextDeinit();

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        onContextInit();
    }
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

void Surface::onResize(int, int)
{
}

void Surface::onRender()
{
}

void Surface::onKeyPress(int)
{
}

void Surface::onKeyRelease(int)
{
}

void Surface::onMouseMove(int, int)
{
}

void Surface::onMousePress(int, int, int)
{
}

void Surface::onMouseRelease(int, int, int)
{
}

void Surface::onTimer(int)
{
}


} // namespace gloperate
