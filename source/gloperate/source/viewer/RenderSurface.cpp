
#include <gloperate/viewer/RenderSurface.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/TimeManager.h>
#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


RenderSurface::RenderSurface(ViewerContext * viewerContext, Stage * stage)
: Surface(viewerContext)
, m_renderStage(nullptr)
, m_frame(0)
{
    setRenderStage(stage);
}

RenderSurface::~RenderSurface()
{
    delete m_renderStage;
}

Stage * RenderSurface::renderStage() const
{
    return m_renderStage;
}

void RenderSurface::setRenderStage(Stage * stage)
{
    // Destroy old render stage
    if (m_renderStage)
    {
        // De-initialize render stage
        m_renderStage->deinitContext(m_openGLContext);

        // Disconnect from events
//      m_renderStage->outputInvalidated.disconnect(this);

        // Destroy render stage
        delete m_renderStage;
    }

    // Set new render stage
    m_renderStage = stage;
    if (m_renderStage)
    {
        // Connect to events
        m_renderStage->outputInvalidated.connect([this] ()
        {
            redrawNeeded();
        });

        // Initialize render stage
        m_renderStage->initContext(m_openGLContext);
    }
}

const GLContextFormat & RenderSurface::requiredFormat() const
{
    if (m_renderStage) {
        return m_renderStage->requiredFormat();
    } else {
        return Surface::requiredFormat();
    }
}

void RenderSurface::onUpdate()
{
    if (m_renderStage)
    {
        m_renderStage->setTimeDelta(m_viewerContext->timeManager()->timeDelta());
    }
}

void RenderSurface::onContextInit()
{
    globjects::info() << "onContextInit()";

    // Initialize render stage in new context
    if (m_renderStage)
    {
        m_renderStage->initContext(m_openGLContext);
    }
}

void RenderSurface::onContextDeinit()
{
    globjects::info() << "onContextDeinit()";

    // De-initialize render stage in old context
    if (m_renderStage)
    {
        m_renderStage->deinitContext(m_openGLContext);
    }
}

void RenderSurface::onViewport(const glm::ivec4 & deviceViewport, const glm::ivec4 & virtualViewport)
{
    m_renderStage->setDeviceViewport(
        deviceViewport.x
      , deviceViewport.y
      , deviceViewport.z
      , deviceViewport.w
    );

    m_renderStage->setVirtualViewport(
        virtualViewport.x
      , virtualViewport.y
      , virtualViewport.z
      , virtualViewport.w
    );
}

void RenderSurface::onRender()
{
//  globjects::info() << "onRender()";

    if (m_renderStage)
    {
        m_frame++;
        m_renderStage->setFrameCounter(m_frame);
        m_renderStage->process(m_openGLContext);
    }
}

void RenderSurface::onKeyPress(int key, int modifier)
{
    auto inputEvent = new gloperate::ButtonEvent{
        gloperate::InputEvent::Type::ButtonPress,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_viewerContext->inputManager()->onEvent(inputEvent);

    globjects::info() << "onKeyPressed(" << key << ")";
}

void RenderSurface::onKeyRelease(int key, int modifier)
{
    auto inputEvent = new gloperate::ButtonEvent{
        gloperate::InputEvent::Type::ButtonRelease,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_viewerContext->inputManager()->onEvent(inputEvent);

    globjects::info() << "onKeyReleased(" << key << ")";
}

void RenderSurface::onMouseMove(const glm::ivec2 & pos)
{
    globjects::info() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMousePress(int button, const glm::ivec2 & pos)
{
    globjects::info() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseRelease(int button, const glm::ivec2 & pos)
{
    globjects::info() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    globjects::info() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";
}


} // namespace gloperate
