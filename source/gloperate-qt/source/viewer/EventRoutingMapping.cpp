#include <gloperate-qt/viewer/EventRoutingMapping.h>

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


using namespace gloperate;

using gloperate::make_unique;


namespace
{
    const int g_tooltipTimeout = 200;
}


namespace gloperate_qt
{


EventRoutingMapping::EventRoutingMapping(QtOpenGLWindow * window)
: AbstractQtMapping(window)
, m_viewportCapability(nullptr)
, m_typedRenderTargetCapability(nullptr)
{
}

EventRoutingMapping::~EventRoutingMapping()
{
}

void EventRoutingMapping::initializeTools()
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

void EventRoutingMapping::mapEvent(AbstractEvent * event)
{
    if (m_renderTarget && !m_renderTarget->hasRenderTarget(RenderTargetType::Depth))
        onTargetFramebufferChanged();

    switch (event->sourceType())
    {
    case EventType::Keyboard:
        mapKeyboardEvent(static_cast<KeyboardEvent *>(event));
        break;
    case EventType::Mouse:
        mapMouseEvent(static_cast<MouseEvent *>(event));
        break;
    case EventType::Wheel:
        mapWheelEvent(static_cast<WheelEvent *>(event));
        break;
    default:
        break;
    }
}

void EventRoutingMapping::mapKeyboardEvent(KeyboardEvent * event)
{
    if (event && event->type() == KeyboardEvent::Type::Press)
    {
        auto curReciever = mapEventToReciever(RoutingEventType::Keyboard, static_cast<int>(RoutingEventValue::Any));
        curReciever->keyPress(KeyboardInteractionArgs{ event->key(), event->scanCode(), event->modifiers() });
    }
}

void EventRoutingMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    if (!mouseEvent) return;

    m_currentMousePosition = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
    m_currentId = ObjectIdExtractor(m_viewportCapability, m_typedRenderTargetCapability).get(m_currentMousePosition);

    if (mouseEvent->type() == MouseEvent::Type::Press)
    {
        MouseButton curButton = mouseEvent->button();
        //mark button as pressed
        pressedButtons.insert(curButton);
        
        auto curReciever = mapEventToReciever(RoutingEventType::MouseButton, curButton);
        //nobody subcribed for this event
        if (!curReciever || m_eventReciever.count(curButton))
            return;

        //remember the reciever for this button
        m_eventReciever[curButton] = InteractionData{ curReciever, m_currentMousePosition };
    }
    else if (mouseEvent->type() == MouseEvent::Type::Move)
    {
        handleMouseMoveEvent();
    }
    else if (mouseEvent->type() == MouseEvent::Type::Release)
    {
        MouseButton curButton = mouseEvent->button();
        //mark button as released
        pressedButtons.erase(curButton);

        if (!m_eventReciever.count(curButton)) return;

        handleMouseReleaseEvent(curButton);

        m_eventReciever.erase(curButton);
    }
}

void EventRoutingMapping::handleMouseMoveEvent()
{
    for (auto btn = pressedButtons.begin(); btn != pressedButtons.end(); ++btn) {
        if (!m_eventReciever.count(*btn)) continue;
        InteractionData &reciever = m_eventReciever.at(*btn);
        glm::ivec2 delta = m_currentMousePosition - m_eventReciever.at(*btn).startPosition;
        if (reciever.state != DragStarted) {
            reciever.reciever->dragStart(MouseInteractionArgs{ reciever.startPosition, *btn });
            reciever.state = DragStarted;
        }
        auto hoverReciever = mapEventToReciever(RoutingEventType::Any, static_cast<int>(RoutingEventValue::Any));
        AbstractInteraction* hoverElement = reciever.reciever != hoverReciever ? hoverReciever : nullptr;
        reciever.reciever->dragDelta(MouseInteractionDragDeltaArgs{ m_currentMousePosition, *btn, hoverElement, delta, reciever.startPosition });
    }
}

void EventRoutingMapping::handleMouseReleaseEvent(MouseButton value)
{
    if (!m_eventReciever.count(value)) return;

    InteractionData curReciever = m_eventReciever[value];
    glm::ivec2 delta = m_currentMousePosition - curReciever.startPosition;
    if (delta == glm::ivec2(0, 0))
    {
        curReciever.reciever->click(MouseInteractionArgs{ m_currentMousePosition, static_cast<MouseButton>(value) });
    }
    else
    {
        curReciever.reciever->dragEnd(MouseInteractionArgs{ m_currentMousePosition, static_cast<MouseButton>(value) });
    }
}

void EventRoutingMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    m_currentMousePosition = wheelEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
    auto reciever = mapEventToReciever(RoutingEventType::MouseWheel, static_cast<int>(RoutingEventValue::Any));
    if (reciever) {
        reciever->mouseWheel(MouseInteractionWheelArgs{ m_currentMousePosition, wheelEvent->angleDelta() });
    }
}

void EventRoutingMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    m_renderTarget->setRenderTarget(RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}

AbstractInteraction* EventRoutingMapping::mapEventToReciever(RoutingEventType eventType, int eventValue) const
{
    // request registered eventReciever
    AbstractInteraction* curReciever = m_eventRoutingCapability ? m_eventRoutingCapability->mapEventToReciever(m_currentId, eventType, eventValue) : nullptr;
    
    if (!curReciever)
        // forward event to navigation
        curReciever = m_eventRoutingCapability->getNavigation();
    return curReciever;
}
} // namespace gloperate_qt
