
#include <gloperate/viewer/RenderSurface.h>

#include <cppassist/logging/logging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/TimeManager.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>
#include <gloperate/output/AbstractVideoTool.h>


namespace gloperate
{


RenderSurface::RenderSurface(ViewerContext * viewerContext)
: Surface(viewerContext)
, m_renderStage(nullptr)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_keyboardDevice(new KeyboardDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_video(nullptr)
, m_requestVideo(false)
, m_deviceViewport(0)
, m_virtualViewport(0)
{
    addFunction("createVideo", this, &RenderSurface::createVideo);

    if (m_viewerContext->scriptEnvironment())
    {
        m_viewerContext->scriptEnvironment()->addApi(this);
    }
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
        if (m_openGLContext) {
            m_renderStage->deinitContext(m_openGLContext);
        }

        // [TODO] Disconnect from events
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
        if (m_openGLContext) {
            m_renderStage->initContext(m_openGLContext);
        }
    }
}

void RenderSurface::setVideoTool(AbstractVideoTool * video)
{
    m_video = video;
}

void RenderSurface::createVideo(std::string filename, int fps, int seconds, int width, int height)
{
    globjects::debug() << "<----- Creating Video ----->";

    m_video->init(filename, this, fps, seconds, width, height);
    m_requestVideo = true;
}

glm::ivec4 RenderSurface::deviceViewport()
{
    return m_deviceViewport;
}

glm::ivec4 RenderSurface::virtualViewport()
{
    return m_virtualViewport;
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
    cppassist::info() << "onContextInit()";

    // Initialize render stage in new context
    if (m_renderStage)
    {
        m_renderStage->initContext(m_openGLContext);
    }
}

void RenderSurface::onContextDeinit()
{
    cppassist::info() << "onContextDeinit()";

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

    m_deviceViewport = deviceViewport;
    m_virtualViewport = virtualViewport;
}

void RenderSurface::onBackgroundColor(float red, float green, float blue)
{
    m_renderStage->setBackgroundColor(red, green, blue);
}

void RenderSurface::onRender()
{
//  cppassist::info() << "onRender()";
    if (m_requestVideo)
    {
        m_requestVideo = false;
        m_video->createVideo([] (int x, int y) { globjects::debug() << "Progress: " << x*100/y <<"%"; }, true);
    }

    if (m_renderStage)
    {
        m_frame++;
        m_renderStage->setFrameCounter(m_frame);
        m_renderStage->process(m_openGLContext);
    }
}

void RenderSurface::onKeyPress(int key, int modifier)
{
    m_keyboardDevice->keyPress(key, modifier);
    cppassist::info() << "onKeyPressed(" << key << ")";
}

void RenderSurface::onKeyRelease(int key, int modifier)
{
    m_keyboardDevice->keyRelease(key, modifier);
    cppassist::info() << "onKeyReleased(" << key << ")";
}

void RenderSurface::onMouseMove(const glm::ivec2 & pos)
{
    m_mouseDevice->move(pos);
    cppassist::info() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMousePress(int button, const glm::ivec2 & pos)
{
    m_mouseDevice->buttonPress(button, pos);
    cppassist::info() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseRelease(int button, const glm::ivec2 & pos)
{
    m_mouseDevice->buttonRelease(button, pos);
    cppassist::info() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";
}

void RenderSurface::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    m_mouseDevice->wheelScroll(delta, pos);
    cppassist::info() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";
}


} // namespace gloperate
