
#include <gloperate/base/Canvas.h>

#include <cppassist/logging/logging.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/TimeManager.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>


using namespace cppassist;


namespace gloperate
{


Canvas::Canvas(Environment * environment)
: AbstractCanvas(environment)
, m_pipelineContainer(environment)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_environment->inputManager(), m_name))
, m_keyboardDevice(new KeyboardDevice(m_environment->inputManager(), m_name))
{
    // Mark render output as required and redraw when it is invalidated
    m_pipelineContainer.rendered.setRequired(true);
    m_pipelineContainer.rendered.valueChanged.connect([this] (bool)
    {
        if (!m_pipelineContainer.rendered.isValid()) {
            this->redraw();
        }
    });

    addProperty(&m_pipelineContainer);
}

Canvas::~Canvas()
{
}

PipelineContainer * Canvas::pipelineContainer() const
{
    return const_cast<PipelineContainer *>(&m_pipelineContainer);
}

Stage * Canvas::renderStage() const
{
    return m_pipelineContainer.renderStage();
}

void Canvas::setRenderStage(Stage * stage)
{
    // De-initialize render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->deinitContext(m_openGLContext);
    }

    // Set new render stage
    m_pipelineContainer.setRenderStage(stage);

    // Initialize new render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }
}

void Canvas::onUpdate()
{
    float timeDelta = m_environment->timeManager()->timeDelta();

    m_pipelineContainer.timeDelta.setValue(timeDelta);
}

void Canvas::onContextInit()
{
    cppassist::details() << "onContextInit()";

    // Initialize render stage in new context
    if (m_pipelineContainer.renderStage())
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }
}

void Canvas::onContextDeinit()
{
    cppassist::details() << "onContextDeinit()";

    // De-initialize render stage in old context
    if (m_pipelineContainer.renderStage())
    {
        m_pipelineContainer.renderStage()->deinitContext(m_openGLContext);
    }
}

void Canvas::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_deviceViewport  = deviceViewport;
    m_virtualViewport = virtualViewport;

    m_pipelineContainer.deviceViewport.setValue(deviceViewport);
    m_pipelineContainer.virtualViewport.setValue(virtualViewport);
}

void Canvas::onSaveViewport()
{
    m_savedDeviceVP  = m_deviceViewport;
    m_savedVirtualVP = m_virtualViewport;
}

void Canvas::onResetViewport()
{
    onViewport(m_savedDeviceVP, m_savedVirtualVP);
}

void Canvas::onBackgroundColor(float red, float green, float blue)
{
    m_pipelineContainer.backgroundColor.setValue(glm::vec3(red, green, blue));
}

void Canvas::onRender(globjects::Framebuffer * targetFBO)
{
    cppassist::details() << "onRender()";

    // Invoke image and video exports
    AbstractCanvas::onRender(targetFBO);

    // Invoke render stage/pipeline
    if (m_pipelineContainer.renderStage())
    {
        m_frame++;

        m_pipelineContainer.frameCounter.setValue(m_frame);
        m_pipelineContainer.targetFBO.setValue(targetFBO);

        m_pipelineContainer.renderStage()->process(m_openGLContext);
    }
}

void Canvas::onKeyPress(int key, int modifier)
{
    cppassist::details() << "onKeyPressed(" << key << ")";

    m_keyboardDevice->keyPress(key, modifier);
}

void Canvas::onKeyRelease(int key, int modifier)
{
    cppassist::details() << "onKeyReleased(" << key << ")";

    m_keyboardDevice->keyRelease(key, modifier);
}

void Canvas::onMouseMove(const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";

    m_mouseDevice->move(pos);
}

void Canvas::onMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonPress(button, pos);
}

void Canvas::onMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonRelease(button, pos);
}

void Canvas::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->wheelScroll(delta, pos);
}


} // namespace gloperate
