
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

AbstractGLContext * Surface::openGLContext() const
{
    return m_openGLContext;
}

void Surface::setOpenGLContext(AbstractGLContext * context)
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

const GLContextFormat & Surface::requiredFormat() const
{
    static GLContextFormat defaultFormat;

    return defaultFormat;
}

void Surface::onContextInit()
{
}

void Surface::onContextDeinit()
{
}

void Surface::onResize(const glm::ivec2 &, const glm::ivec2 &)
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

void Surface::onMouseMove(const glm::ivec2 &)
{
}

void Surface::onMousePress(int, const glm::ivec2 &)
{
}

void Surface::onMouseRelease(int, const glm::ivec2 &)
{
}

void Surface::onMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
}


} // namespace gloperate
