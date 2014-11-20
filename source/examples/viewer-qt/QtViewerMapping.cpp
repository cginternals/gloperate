#include "QtViewerMapping.h"

#include <gloperate/Camera.h>
#include <gloperate/capabilities/CameraCapability.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/capabilities/AbstractCoordinateProviderCapability.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/navigation/WorldInHandNavigation.h>
#include <gloperate/Painter.h>


using namespace gloperate;

QtViewerMapping::QtViewerMapping()
{
}

QtViewerMapping::~QtViewerMapping()
{
}

void QtViewerMapping::initializeNavigation()
{
    if (    m_painter && 
            m_painter->supports<AbstractCameraCapability>() && 
            m_painter->supports<AbstractCoordinateProviderCapability>() &&
            m_painter->supports<AbstractViewportCapability>())
    {
        AbstractCameraCapability * cameraCapability = dynamic_cast<AbstractCameraCapability*>(m_painter->getCapability<AbstractCameraCapability>());
        AbstractCoordinateProviderCapability * coordProviderCapability = dynamic_cast<AbstractCoordinateProviderCapability*>(m_painter->getCapability<AbstractCoordinateProviderCapability>());
        AbstractViewportCapability * viewportCapability = dynamic_cast<AbstractViewportCapability*>(m_painter->getCapability<AbstractViewportCapability>());
        m_navigation.reset(new WorldInHandNavigation(*cameraCapability, *viewportCapability, *coordProviderCapability));
    }
}

void QtViewerMapping::processEvent(AbstractEvent * event)
{
    if (event->sourceType() == gloperate::SourceType::Keyboard)
    {
        KeyboardEvent * keyEvent = dynamic_cast<KeyboardEvent*>(event);
        if (keyEvent && keyEvent->type() == KeyboardEvent::Type::Press)
        {
            switch (keyEvent->key())
            {
            // WASD move camera
            case KeyW:
                m_navigation->pan(glm::vec3(0, 0, 1));
                break;
            case KeyA:
                m_navigation->pan(glm::vec3(1, 0, 0));
                break;
            case KeyS:
                m_navigation->pan(glm::vec3(0, 0, -1));
                break;
            case KeyD:
                m_navigation->pan(glm::vec3(-1, 0, 0));
                break;
            // Reset camera position
            case KeyR:
                m_navigation->reset();
                break;
            // Arrows rotate camera
            case KeyUp:
                m_navigation->rotate(0.0f, glm::radians(-10.0f));
                break;
            case KeyLeft:
                m_navigation->rotate(glm::radians(10.0f), 0.0f);
                break;
            case KeyDown:
                m_navigation->rotate(0.0f, glm::radians(10.0f));
                break;
            case KeyRight:
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
            case MouseButtonMiddle:
                m_navigation->reset();
                break;
            case MouseButtonLeft:
                m_navigation->panBegin(mouseEvent->pos());
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
                m_navigation->panProcess(mouseEvent->pos());
            default:
                break;
            }
        }
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Release)
        {
            switch (mouseEvent->button())
            {
            case MouseButtonLeft:
                    m_navigation->panEnd();
            default:
                break;
            }
        }
    }
}

