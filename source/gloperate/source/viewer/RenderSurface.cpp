
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
, deviceViewport (this, "deviceViewport",  glm::vec4(0, 0, 0, 0))
, virtualViewport(this, "virtualViewport", glm::vec4(0, 0, 0, 0))
, backgroundColor(this, "backgroundColor", glm::vec3(1.0, 1.0, 1.0))
, frameCounter   (this, "frameCounter",    0)
, timeDelta      (this, "timeDelta",       0.0f)
, redrawNeeded   (this, "redrawNeeded")
, m_renderStage(nullptr)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_viewerContext->inputManager(), "Render Surface"))
, m_keyboardDevice(new KeyboardDevice(m_viewerContext->inputManager(), "Render Surface"))
{
    redrawNeeded.valueChanged.connect([this] (bool needRedraw)
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
        redrawNeeded.disconnect();

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
        connect(m_renderStage, "deviceViewport",  &this->deviceViewport);
        connect(m_renderStage, "virtualViewport", &this->virtualViewport);
        connect(m_renderStage, "backgroundColor", &this->backgroundColor);
        connect(m_renderStage, "frameCounter",    &this->frameCounter);
        connect(m_renderStage, "timeDelta",       &this->timeDelta);
        connect(&this->redrawNeeded, m_renderStage, "redrawNeeded");

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
        timeDelta.setValue(m_viewerContext->timeManager()->timeDelta());
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

void RenderSurface::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    this->deviceViewport.setValue(deviceViewport);
    this->virtualViewport.setValue(virtualViewport);
}

void RenderSurface::onBackgroundColor(float red, float green, float blue)
{
    this->backgroundColor.setValue(glm::vec3(red, green, blue));
}

void RenderSurface::onRender()
{
//  cppassist::info() << "onRender()";

    if (m_renderStage)
    {
        m_frame++;

        frameCounter.setValue(m_frame);

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
