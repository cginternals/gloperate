
#include <globjects/base/baselogging.h>

#include <gloperate/viewer/Surface.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


static int s_surfaces = 0;


Surface::Surface(ViewerContext * viewerContext)
: cppexpose::Object("surface" + std::to_string(s_surfaces++))
, m_viewerContext(viewerContext)
, m_openGLContext(nullptr)
{
    m_viewerContext->registerSurface(this);
}

Surface::~Surface()
{
    m_viewerContext->unregisterSurface(this);
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

glm::vec4 Surface::deviceViewport()
{
}

glm::vec4 Surface::virtualViewport()
{
}

void Surface::onUpdate()
{
}

void Surface::onContextInit()
{
}

void Surface::onContextDeinit()
{
}

void Surface::onViewport(const glm::vec4 &, const glm::vec4 &)
{
}

void Surface::onBackgroundColor(float, float, float)
{
}

void Surface::onRender()
{
}

void Surface::onKeyPress(int, int)
{
}

void Surface::onKeyRelease(int, int)
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
