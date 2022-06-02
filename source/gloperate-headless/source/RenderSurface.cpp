
#include <gloperate-headless/RenderSurface.h>

#include <glm/glm.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Canvas.h>
#include <gloperate/base/Environment.h>

#include <gloperate-headless/Application.h>
#include <gloperate-headless/GLContext.h>


using namespace gloperate;


namespace gloperate_headless
{


RenderSurface::RenderSurface(gloperate::Environment * environment)
: Surface(Application::s_app->m_display)
, m_environment(environment)
, m_canvas(cppassist::make_unique<Canvas>(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        repaint();
    } );

    // TODO: ?
    /*m_canvas->setViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
    );*/
}

RenderSurface::~RenderSurface()
{
}

const gloperate::Environment * RenderSurface::environment() const
{
    return m_environment;
}

gloperate::Environment * RenderSurface::environment()
{
    return m_environment;
}

gloperate::Canvas * RenderSurface::canvas() const
{
    return m_canvas.get();
}

void RenderSurface::onContextInit()
{
    m_canvas->setOpenGLContext(m_context.get());
}

void RenderSurface::onContextDeinit()
{
    m_canvas->setOpenGLContext(nullptr);
}

void RenderSurface::onIdle()
{
    m_canvas->updateTime();
}


} // namespace gloperate_headless
