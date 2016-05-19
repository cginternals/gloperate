
#include <gloperate/viewer/RenderSurface.h>

#include <cppassist/logging/logging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/TimeManager.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>
#include <gloperate/output/AbstractVideoTool.h>
#include <gloperate/tools/ImageExporter.h>


namespace gloperate
{


RenderSurface::RenderSurface(ViewerContext * viewerContext)
: Surface(viewerContext)
, m_viewer(viewerContext)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_keyboardDevice(new KeyboardDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_video(nullptr)
, m_imageExporter(nullptr)
, m_requestVideo(false)
{
    addFunction("createVideo", this, &RenderSurface::createVideo);
    addFunction("exportImage", this, &RenderSurface::exportImage);

    if (m_viewerContext->scriptEnvironment())
    {
        m_viewerContext->scriptEnvironment()->addApi(this);
    }

    m_viewer.outputs.rendered.valueChanged.connect([this] (bool rendered)
    {
        if (!rendered) {
            this->redraw();
        }
    });
}

RenderSurface::~RenderSurface()
{
    delete m_imageExporter;
}

Pipeline * RenderSurface::rootPipeline() const
{
    return static_cast<Pipeline *>(const_cast<ViewerContainer *>(&m_viewer));
}

Stage * RenderSurface::renderStage() const
{
    return m_viewer.renderStage();
}

void RenderSurface::setRenderStage(Stage * stage)
{
    // De-initialize render stage
    if (m_viewer.renderStage() && m_openGLContext)
    {
        m_viewer.renderStage()->deinitContext(m_openGLContext);
    }

    // Set new render stage
    m_viewer.setRenderStage(stage);

    // Initialize new render stage
    if (m_viewer.renderStage() && m_openGLContext)
    {
        m_viewer.renderStage()->initContext(m_openGLContext);
    }
}

void RenderSurface::setVideoTool(AbstractVideoTool * video)
{
    m_video = video;
}

void RenderSurface::createVideo(std::string filename, int width, int height, int fps, int seconds)
{
    cppassist::debug() << "<----- Creating Video ----->";

    m_video->init(filename, this, width, height, fps, seconds);
    m_requestVideo = true;
}

void RenderSurface::exportImage(std::string filename, int width, int height, int renderIterations)
{
    m_imageExporter->init(filename, width, height, renderIterations);
    m_requestImage = true;
}

glm::vec4 RenderSurface::deviceViewport()
{
    return m_viewer.inputs.deviceViewport.value();
}

glm::vec4 RenderSurface::virtualViewport()
{
    return m_viewer.inputs.virtualViewport.value();
}

void RenderSurface::onUpdate()
{
    m_viewer.inputs.timeDelta.setValue(m_viewerContext->timeManager()->timeDelta());
}

void RenderSurface::onContextInit()
{
    cppassist::details() << "onContextInit()";

    // Initialize render stage in new context
    if (m_viewer.renderStage())
    {
        m_viewer.renderStage()->initContext(m_openGLContext);
    }

    if (!m_imageExporter)
    {
        m_imageExporter = new ImageExporter(this);
    }
}

void RenderSurface::onContextDeinit()
{
    cppassist::details() << "onContextDeinit()";

    // De-initialize render stage in old context
    if (m_viewer.renderStage())
    {
        m_viewer.renderStage()->deinitContext(m_openGLContext);
    }
}

void RenderSurface::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_viewer.inputs.deviceViewport.setValue(deviceViewport);
    m_viewer.inputs.virtualViewport.setValue(virtualViewport);
}

void RenderSurface::onBackgroundColor(float red, float green, float blue)
{
    m_viewer.inputs.backgroundColor.setValue(glm::vec3(red, green, blue));
}

void RenderSurface::onRender(globjects::Framebuffer * targetFBO)
{
    cppassist::details() << "onRender()";

    if (m_requestVideo)
    {
        m_requestVideo = false;
        m_video->createVideo([] (int x, int y) { cppassist::debug() << "Progress: " << x*100/y <<"%"; }, true);
    }

    if (m_requestImage)
    {
        m_requestImage = false;
        m_imageExporter->save(true);
    }

    if (m_viewer.renderStage())
    {
        m_frame++;

        m_viewer.inputs.frameCounter.setValue(m_frame);
        m_viewer.inputs.targetFBO.setValue(targetFBO);

        m_viewer.renderStage()->process(m_openGLContext);
    }
}

void RenderSurface::onKeyPress(int key, int modifier)
{
    cppassist::details() << "onKeyPressed(" << key << ")";

    m_keyboardDevice->keyPress(key, modifier);
}

void RenderSurface::onKeyRelease(int key, int modifier)
{
    cppassist::details() << "onKeyReleased(" << key << ")";

    m_keyboardDevice->keyRelease(key, modifier);
}

void RenderSurface::onMouseMove(const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";

    m_mouseDevice->move(pos);
}

void RenderSurface::onMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonPress(button, pos);
}

void RenderSurface::onMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonRelease(button, pos);
}

void RenderSurface::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->wheelScroll(delta, pos);
}


} // namespace gloperate
