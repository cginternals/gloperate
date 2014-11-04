#include "QtViewerMapping.h"

#include <QDebug>

#include <gloperate/Camera.h>
#include <gloperate/capabilities/CameraCapability.h>
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
    if (m_painter && m_painter->supports<CameraCapability>())
    {
        CameraCapability * cameraCapability = dynamic_cast<CameraCapability*>(m_painter->getCapability<CameraCapability>());
        m_navigation.reset(new WorldInHandNavigation(cameraCapability->getCamera()));
    }
}

void QtViewerMapping::processEvent(AbstractEvent * event)
{
    if (event->sourceType() == gloperate::SourceType::Keyboard)
    {
        KeyboardEvent * keyEvent = dynamic_cast<KeyboardEvent*>(event);
        if (keyEvent)
        {
            switch (keyEvent->key())
            {
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
            case KeyR:
                m_navigation->reset(true);
                break;
            default:
                break;
            }
        }
    }
    else if (event->sourceType() == gloperate::SourceType::Mouse)
    {
        MouseEvent * mouseEvent = dynamic_cast<MouseEvent*>(event);
        if (mouseEvent)
        {
            if (mouseEvent->button() == gloperate::MouseButton1)
            {
                qDebug() << "'MouseButton1' was pressed!";
            }
        }
    }
}

