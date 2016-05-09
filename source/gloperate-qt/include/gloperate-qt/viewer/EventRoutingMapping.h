#pragma once

#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <gloperate/ext-includes-end.h>

#include <glm/glm.hpp>

#include <gloperate-qt/viewer/AbstractQtMapping.h>
#include <gloperate/navigation/AbstractEventRoutingCapability.h>
#include <gloperate/input/input.h>


class QTimer;

namespace gloperate
{
    class CoordinateProvider;
    class TypedRenderTargetCapability;
    class AbstractMetaInformationCapability;
    class AbstractViewportCapability;
    class WorldInHandNavigation;
    class KeyboardEvent;
    class MouseEvent;
    class WheelEvent;
}


namespace gloperate_qt
{


class GLOPERATE_QT_API EventRoutingMapping :public gloperate_qt::AbstractQtMapping
{
    
enum InteractionState
{
    NoDrag
    , DragStarted
};

struct InteractionData
{
    gloperate::AbstractInteraction * reciever = nullptr;
    InteractionState state;
    glm::ivec2 startPosition;
    InteractionData(gloperate::AbstractInteraction* reciever = nullptr, glm::ivec2 startPosition = glm::ivec2(0.0)) :
        reciever(reciever), state(NoDrag), startPosition(startPosition) {};
};


public:
    EventRoutingMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~EventRoutingMapping();

    virtual void initializeTools() override;


protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

    void mapKeyboardEvent(gloperate::KeyboardEvent * event);
    void mapMouseEvent(gloperate::MouseEvent * event);
    void handleMouseMoveEvent();
    void handleMouseReleaseEvent(int value);
    void mapWheelEvent(gloperate::WheelEvent * event);

    void onTargetFramebufferChanged();

protected:
    gloperate::AbstractInteraction* mapToReciever(gloperate::RoutingEventType type, int value) const;
    std::unique_ptr<gloperate::TypedRenderTargetCapability> m_renderTarget;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    gloperate::AbstractEventRoutingCapability * m_eventRoutingCapability;
    glm::ivec2 m_currentMousePosition;
    int m_currentId;
    std::unordered_map<gloperate::MouseButton, InteractionData> m_eventReciever;
    std::set<gloperate::MouseButton> pressedButtons;
};


} // namespace gloperate_qt
