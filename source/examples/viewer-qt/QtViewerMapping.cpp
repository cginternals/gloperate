#include "QtViewerMapping.h"

#include <gloperate/painter/Camera.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/WheelEvent.h>
#include <gloperate/navigation/WorldInHandNavigation.h>
#include <gloperate/tools/CoordinateProvider.h>


using namespace gloperate;

QtViewerMapping::QtViewerMapping()
{
}

QtViewerMapping::~QtViewerMapping()
{
}

void QtViewerMapping::initializeTools()
{
    if (    m_painter &&
            m_painter->supports<AbstractCameraCapability>() &&
            m_painter->supports<AbstractViewportCapability>() &&
            m_painter->supports<AbstractTypedRenderTargetCapability>() &&
            m_painter->supports<AbstractProjectionCapability>())
    {
        AbstractCameraCapability * cameraCapability = dynamic_cast<AbstractCameraCapability*>(m_painter->getCapability<AbstractCameraCapability>());
        AbstractProjectionCapability * projectionCapability = dynamic_cast<AbstractProjectionCapability*>(m_painter->getCapability<AbstractProjectionCapability>());
        AbstractTypedRenderTargetCapability * renderTargetCapability = dynamic_cast<AbstractTypedRenderTargetCapability*>(m_painter->getCapability<AbstractTypedRenderTargetCapability>());
        AbstractViewportCapability * viewportCapability = dynamic_cast<AbstractViewportCapability*>(m_painter->getCapability<AbstractViewportCapability>());

        m_coordProvider.reset(new CoordinateProvider(cameraCapability, projectionCapability, viewportCapability, renderTargetCapability));
        m_navigation.reset(new WorldInHandNavigation(*cameraCapability, *viewportCapability, *m_coordProvider));
    }
}

void QtViewerMapping::mapEvent(AbstractEvent * event)
{
    if (event->sourceType() == gloperate::SourceType::Keyboard)
    {
        KeyboardEvent * keyEvent = dynamic_cast<KeyboardEvent*>(event);
        if (keyEvent && keyEvent->type() == KeyboardEvent::Type::Press)
        {
            switch (keyEvent->key())
            {
            // WASD move camera
            case Key::KeyW:
                m_navigation->pan(glm::vec3(0, 0, 1));
                break;
            case Key::KeyA:
                m_navigation->pan(glm::vec3(1, 0, 0));
                break;
            case Key::KeyS:
                m_navigation->pan(glm::vec3(0, 0, -1));
                break;
            case Key::KeyD:
                m_navigation->pan(glm::vec3(-1, 0, 0));
                break;
            // Reset camera position
            case Key::KeyR:
                m_navigation->reset();
                break;
            // Arrows rotate camera
            case Key::KeyUp:
                m_navigation->rotate(0.0f, glm::radians(-10.0f));
                break;
            case Key::KeyLeft:
                m_navigation->rotate(glm::radians(10.0f), 0.0f);
                break;
            case Key::KeyDown:
                m_navigation->rotate(0.0f, glm::radians(10.0f));
                break;
            case Key::KeyRight:
                m_navigation->rotate(glm::radians(-10.0f), 0.0f);
                break;
            default:
                break;
            }
        }
    }
    else if (event->sourceType() == gloperate::SourceType::Mouse)
    {
        MouseEvent * mouseEvent = dynamic_cast<MouseEvent*>(event);
        if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Press)
        {
            switch (mouseEvent->button())
            {
            case MouseButton::MouseButtonMiddle:
                m_navigation->reset();
                break;
            case MouseButton::MouseButtonLeft:
                m_navigation->panBegin(mouseEvent->position());
                break;
            case MouseButton::MouseButtonRight:
                m_navigation->rotateBegin(mouseEvent->position());
                break;
            default:
                break;
            }
        }
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
        {
            switch (m_navigation->mode())
            {
            case WorldInHandNavigation::InteractionMode::PanInteraction:
                m_navigation->panProcess(mouseEvent->position());
                break;
            case WorldInHandNavigation::InteractionMode::RotateInteraction:
                m_navigation->rotateProcess(mouseEvent->position());
                break;
            default:
                break;
            }
        }
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Release)
        {
            switch (mouseEvent->button())
            {
            case MouseButton::MouseButtonLeft:
                m_navigation->panEnd();
                break;
            case MouseButton::MouseButtonRight:
                m_navigation->rotateEnd();
                break;
            default:
                break;
            }
        }
    }
    else if (event->sourceType() == gloperate::SourceType::Wheel)
    {
        WheelEvent * wheelEvent = dynamic_cast<WheelEvent*>(event);
        if (wheelEvent)
        {
            auto scale = wheelEvent->angleDelta().y;
            scale /= WheelEvent::defaultMouseAngleDelta();
            scale *= 0.1f; // smoother (slower) scaling
            m_navigation->scaleAtMouse(wheelEvent->pos(), scale);
        }
    }
}

