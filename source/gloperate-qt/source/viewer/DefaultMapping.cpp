
#include <gloperate-qt/viewer/DefaultMapping.h>

#include <gloperate/ext-includes-begin.h>
#include <QTimer>
#include <QToolTip>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/enum.h>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/base/RenderTargetType.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/AbstractMetaInformationCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/WheelEvent.h>
#include <gloperate/navigation/WorldInHandNavigation.h>
#include <gloperate/navigation/CoordinateProvider.h>
#include <gloperate/tools/ObjectIdExtractor.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate_qt;
using namespace gloperate;

using gloperate::make_unique;


namespace
{
    const int g_tooltipTimeout = 200;
}


namespace gloperate_qt
{


DefaultMapping::DefaultMapping(QtOpenGLWindow * window)
: AbstractQtMapping(window)
, m_viewportCapability(nullptr)
, m_typedRenderTargetCapability(nullptr)
{
}

DefaultMapping::~DefaultMapping()
{
}

void DefaultMapping::initializeTools()
{
    m_renderTarget = nullptr;
    m_viewportCapability = nullptr;
    m_typedRenderTargetCapability = nullptr;

    if (!m_painter)
    {
        return;
    }

    if (m_painter->supports<AbstractCameraCapability>() &&
        m_painter->supports<AbstractViewportCapability>() &&
        m_painter->supports<AbstractProjectionCapability>() &&
        (m_painter->supports<AbstractTypedRenderTargetCapability>() ||
         m_painter->supports<AbstractTargetFramebufferCapability>()))
    {
        auto cameraCapability = m_painter->getCapability<AbstractCameraCapability>();
        auto projectionCapability = m_painter->getCapability<AbstractProjectionCapability>();
        m_viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
        
        m_typedRenderTargetCapability = m_painter->getCapability<TypedRenderTargetCapability>();
        if (!m_typedRenderTargetCapability)
        {
            m_renderTarget = make_unique<TypedRenderTargetCapability>();
            m_typedRenderTargetCapability = m_renderTarget.get();

            auto fboCapability = m_painter->getCapability<AbstractTargetFramebufferCapability>();
            fboCapability->changed.connect([this] () { this->onTargetFramebufferChanged(); });
        }
    }
    if (m_painter->supports<AbstractEventRoutingCapability>())
    {
        m_eventRoutingCapability = m_painter->getCapability<AbstractEventRoutingCapability>();
    }
    else
    {
        m_eventRoutingCapability = nullptr;
    }
}

void DefaultMapping::mapEvent(AbstractEvent * event)
{
    if (m_renderTarget && !m_renderTarget->hasRenderTarget(RenderTargetType::Depth))
        onTargetFramebufferChanged();

    switch (event->sourceType())
    {
    case gloperate::EventType::Keyboard:
        mapKeyboardEvent(static_cast<KeyboardEvent *>(event));
        break;
    case gloperate::EventType::Mouse:
        mapMouseEvent(static_cast<MouseEvent *>(event));
        break;
    case gloperate::EventType::Wheel:
        mapWheelEvent(static_cast<WheelEvent *>(event));
        break;
    default:
        break;
    }
}

void DefaultMapping::mapKeyboardEvent(KeyboardEvent * event)
{
    if (event && event->type() == KeyboardEvent::Type::Press)
    {
        auto curReciever = mapToReciever(RoutingEventType::Keyboard, static_cast<int>(RoutingEventValue::Any));
        curReciever->keyPress(KeyboardInteractionArgs(event->key(), event->scanCode(), event->modifiers()));
    }
}

void DefaultMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    if (!mouseEvent) return;

    m_currentMousePosition = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
    m_currentId = ObjectIdExtractor(m_viewportCapability, m_typedRenderTargetCapability).get(m_currentMousePosition);

    if (mouseEvent->type() == MouseEvent::Type::Press)
    {
        gloperate::MouseButton curButton = mouseEvent->button();
        pressedButtons.insert(curButton);
        auto curReciever = mapToReciever(RoutingEventType::MouseButton, curButton);
    
        if (!curReciever || m_eventReciever.count(curButton))
            return;
    
        m_eventReciever[curButton] = InteractionData(curReciever, m_currentMousePosition);
    }
    else if (mouseEvent->type() == MouseEvent::Type::Move)
    {
        handleMouseMoveEvent();
    }
    else if (mouseEvent->type() == MouseEvent::Type::Release)
    {
        gloperate::MouseButton curButton = mouseEvent->button();
        pressedButtons.erase(curButton);

        if (!m_eventReciever.count(curButton)) return;

        handleMouseReleaseEvent(curButton);
        m_eventReciever.erase(curButton);
    }
}

void DefaultMapping::handleMouseMoveEvent()
{
    for (auto btn = pressedButtons.begin(); btn != pressedButtons.end(); ++btn) {
        if (!m_eventReciever.count(*btn)) continue;
        InteractionData &reciever = m_eventReciever.at(*btn);
        glm::ivec2 delta = m_currentMousePosition - m_eventReciever.at(*btn).startPosition;
        if (reciever.state != DragStarted) {
            reciever.reciever->dragStart(MouseInteractionArgs(reciever.startPosition, *btn));
            reciever.state = DragStarted;
        }
        //curReciever is the Element under the mouse no matter what interaction is possible on
        AbstractInteraction* hoverElement = /*reciever.reciever != curReciever ? curReciever : */ nullptr;
        reciever.reciever->dragDelta(MouseInteractionDragDeltaArgs(m_currentMousePosition, *btn, hoverElement, delta, reciever.startPosition));
    }
}

void DefaultMapping::handleMouseReleaseEvent(int value)
{
    InteractionData curReciever = mapToReciever(RoutingEventType::MouseButton, value);
    glm::ivec2 delta = m_currentMousePosition - curReciever.startPosition;
    if (delta == glm::ivec2(0, 0))
    {
        curReciever.reciever->click(MouseInteractionArgs(m_currentMousePosition, static_cast<MouseButton>(value)));
    }
    else
    {
        curReciever.reciever->dragEnd(MouseInteractionArgs(m_currentMousePosition, static_cast<MouseButton>(value)));
    }
}

void DefaultMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    m_currentMousePosition = wheelEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
    auto reciever = mapToReciever(RoutingEventType::MouseWheel, static_cast<int>(RoutingEventValue::Any));
    if (reciever) {
        reciever->mouseWheel(MouseInteractionWheelArgs(m_currentMousePosition, wheelEvent->angleDelta()));
    }
}

void DefaultMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    m_renderTarget->setRenderTarget(gloperate::RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}

gloperate::AbstractInteraction* DefaultMapping::mapToReciever(gloperate::RoutingEventType type, int value) const
{
    AbstractInteraction* curReciever = m_eventRoutingCapability ? m_eventRoutingCapability->mapID(m_currentId, type, value) : nullptr;
    if (!curReciever)
        curReciever = m_eventRoutingCapability->getNavigation();
    return curReciever;
}
} // namespace gloperate_qt
