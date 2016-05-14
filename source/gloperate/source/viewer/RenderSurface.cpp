
#include <gloperate/viewer/RenderSurface.h>

#include <cppassist/logging/logging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/TimeManager.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>


namespace gloperate
{


RenderSurface::RenderSurface(ViewerContext * viewerContext)
: Surface(viewerContext)
, m_viewer(viewerContext)
, m_renderStage(nullptr)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_keyboardDevice(new KeyboardDevice(m_viewerContext->inputManager(), "Render Surface"))
{
    m_viewer.outputs.redrawNeeded.valueChanged.connect([this] (bool needRedraw)
    {
        if (needRedraw) {
            this->redraw();
        }
    });
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
        // Disconnect inputs and outputs from former render stage
        disconnect(m_renderStage, "deviceViewport");
        disconnect(m_renderStage, "virtualViewport");
        disconnect(m_renderStage, "backgroundColor");
        disconnect(m_renderStage, "frameCounter");
        disconnect(m_renderStage, "timeDelta");
        m_viewer.outputs.redrawNeeded.disconnect();

        // De-initialize render stage
        if (m_openGLContext) {
            m_renderStage->deinitContext(m_openGLContext);
        }

        // Destroy render stage
        delete m_renderStage;
    }

    // Set new render stage
    m_renderStage = stage;
    if (m_renderStage)
    {
        // Connect inputs and outputs of render stage
        connect(m_renderStage, "deviceViewport",  &m_viewer.inputs.deviceViewport);
        connect(m_renderStage, "virtualViewport", &m_viewer.inputs.virtualViewport);
        connect(m_renderStage, "backgroundColor", &m_viewer.inputs.backgroundColor);
        connect(m_renderStage, "frameCounter",    &m_viewer.inputs.frameCounter);
        connect(m_renderStage, "timeDelta",       &m_viewer.inputs.timeDelta);
        connect(&m_viewer.outputs.redrawNeeded, m_renderStage, "redrawNeeded");

        // Initialize render stage
        if (m_openGLContext) {
            m_renderStage->initContext(m_openGLContext);
        }
    }
}

void RenderSurface::onUpdate()
{
    if (m_renderStage)
    {
        m_viewer.inputs.timeDelta.setValue(m_viewerContext->timeManager()->timeDelta());
    }
}

void RenderSurface::onContextInit()
{
    cppassist::details() << "onContextInit()";

    // Initialize render stage in new context
    if (m_renderStage)
    {
        m_renderStage->initContext(m_openGLContext);
    }
}

void RenderSurface::onContextDeinit()
{
    cppassist::details() << "onContextDeinit()";

    // De-initialize render stage in old context
    if (m_renderStage)
    {
        m_renderStage->deinitContext(m_openGLContext);
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

void RenderSurface::onRender()
{
    cppassist::details() << "onRender()";

    if (m_renderStage)
    {
        m_frame++;

        m_viewer.inputs.frameCounter.setValue(m_frame);

        m_renderStage->process(m_openGLContext);
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

void RenderSurface::connect(Stage * stage, const std::string & name, const cppexpose::AbstractProperty * source)
{
    // Check source data
    if (!source) {
        return;
    }

    // Get input slot
    AbstractInputSlot * input = dynamic_cast<AbstractInputSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void RenderSurface::connect(AbstractInputSlot * input, Stage * stage, const std::string & name)
{
    // Check input slot
    if (!input) {
        return;
    }

    // Get data container
    const cppexpose::AbstractProperty * source = stage->property(name);
    if (!source) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void RenderSurface::disconnect(Stage * stage, const std::string & name)
{
    // Get input slot
    AbstractInputSlot * input = dynamic_cast<AbstractInputSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Disconnect input slot
    input->disconnect();
}


} // namespace gloperate
