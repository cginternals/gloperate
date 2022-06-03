
#include <gloperate-headless/RenderSurface.h>

#include <glm/glm.hpp>

#include <cppassist/logging/logging.h>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Canvas.h>
#include <gloperate/base/Environment.h>

#include <gloperate-headless/Application.h>
#include <gloperate-headless/GLContext.h>
#include <gloperate-headless/SurfaceEvent.h>


using namespace gloperate;


namespace gloperate_headless
{


RenderSurface::RenderSurface(Application * app, gloperate::Environment * environment)
: Surface(app->m_display)
, m_environment(environment)
, m_canvas(cppassist::make_unique<Canvas>(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        repaint();
    } );
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

void RenderSurface::onResize(ResizeEvent & event)
{
    m_deviceSize = event.size();

    m_canvas->setViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
    );
}

void RenderSurface::onPaint(PaintEvent &)
{
    static const auto defaultFBO = globjects::Framebuffer::defaultFBO();

    cppassist::info("gloperate-headless") << "Surface::onPaint";

    m_canvas->render(defaultFBO.get());
}

void RenderSurface::onKeyPress(KeyEvent & event)
{
    // Skip auto-repeated key events
    if (event.action() == static_cast<unsigned int>(SurfaceEvent::ActionType::Repeat))
    {
        return;
    }

    m_canvas->promoteKeyPress(
        event.key(),
        event.modifiers()
    );
}

void RenderSurface::onKeyRelease(KeyEvent & event)
{
    m_canvas->promoteKeyRelease(
        event.key(),
        event.modifiers()
    );
}

void RenderSurface::onMousePress(MouseEvent & event)
{
    m_canvas->promoteMousePress(
        event.button()
      , event.pos()
      , event.modifiers()
    );
}

void RenderSurface::onMouseRelease(MouseEvent & event)
{
    m_canvas->promoteMouseRelease(
        event.button()
      , event.pos()
      , event.modifiers()
    );
}

void RenderSurface::onMouseMove(MouseEvent & event)
{
    m_canvas->promoteMouseMove(event.pos(), event.modifiers());
}

void RenderSurface::onScroll(MouseEvent & event)
{
    m_canvas->promoteMouseWheel(
        event.delta()
      , event.pos()
      , 0
    );
}

void RenderSurface::onIdle()
{
    m_canvas->updateTime();
}


} // namespace gloperate_headless
