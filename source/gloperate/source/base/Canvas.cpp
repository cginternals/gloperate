
#include <gloperate/base/Canvas.h>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <globjects/Framebuffer.h>

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
, m_mouseDevice(cppassist::make_unique<MouseDevice>(m_environment->inputManager(), m_name))
, m_keyboardDevice(cppassist::make_unique<KeyboardDevice>(m_environment->inputManager(), m_name))
{
    // Mark render output as required and redraw when it is invalidated
    m_pipelineContainer.rendered.setRequired(true);
    m_pipelineContainer.rendered.valueInvalidated.connect([this] ()
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

const PipelineContainer * Canvas::pipelineContainer() const
{
    return &m_pipelineContainer;
}

PipelineContainer * Canvas::pipelineContainer()
{
    return &m_pipelineContainer;
}

const Stage * Canvas::renderStage() const
{
    return m_pipelineContainer.renderStage();
}

Stage * Canvas::renderStage()
{
    return m_pipelineContainer.renderStage();
}

void Canvas::setRenderStage(std::unique_ptr<Stage> && stage)
{
    // De-initialize render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->deinitContext(m_openGLContext);
    }

    // Set new render stage
    m_pipelineContainer.setRenderStage(std::move(stage));

    // Initialize new render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }
}

void Canvas::onRender(globjects::Framebuffer * targetFBO)
{
    if(cppassist::verbosityLevel() < 3)
        cppassist::debug(2, "gloperate") << "onRender()";
    else
    {
        auto fboName = targetFBO->hasName() ? targetFBO->name() : std::to_string(targetFBO->id());
        cppassist::debug(3, "gloperate") << "onRender(); " << "targetFBO: " << fboName;
    }

    // Invoke render stage/pipeline
    if (m_pipelineContainer.renderStage())
    {
        m_frame++;

        m_pipelineContainer.frameCounter.setValue(m_frame);
        m_pipelineContainer.targetFBO.setValue(targetFBO);

        m_pipelineContainer.renderStage()->process(m_openGLContext);
    }
}

void Canvas::onUpdate()
{
    float timeDelta = m_environment->timeManager()->timeDelta();

    m_pipelineContainer.timeDelta.setValue(timeDelta);
}

void Canvas::onContextInit()
{
    cppassist::debug(2, "gloperate") << "onContextInit()";

    // Initialize render stage in new context
    if (m_pipelineContainer.renderStage())
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }
}

void Canvas::onContextDeinit()
{
    cppassist::debug(2, "gloperate") << "onContextDeinit()";

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

void Canvas::onKeyPress(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "onKeyPressed(" << key << ")";

    m_keyboardDevice->keyPress(key, modifier);
}

void Canvas::onKeyRelease(int key, int modifier)
{
    cppassist::debug(2, "gloperate") << "onKeyReleased(" << key << ")";

    m_keyboardDevice->keyRelease(key, modifier);
}

void Canvas::onMouseMove(const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "onMouseMoved(" << pos.x << ", " << pos.y << ")";

    m_mouseDevice->move(pos);
}

void Canvas::onMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonPress(button, pos);
}

void Canvas::onMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonRelease(button, pos);
}

void Canvas::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::debug(2, "gloperate") << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->wheelScroll(delta, pos);
}

const glm::vec4 & Canvas::savedDeviceViewport() const
{
    return m_savedDeviceVP;
}


} // namespace gloperate
