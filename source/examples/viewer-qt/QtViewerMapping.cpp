#include "QtViewerMapping.h"

#include <iostream>

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
#include <gloperate/navigation/TrackballNavigation.h>
#include <gloperate/tools/CoordinateProvider.h>


using namespace gloperate;
using namespace gloperate_qt;

QtViewerMapping::QtViewerMapping(QtOpenGLWindow * window)
    : AbstractQtMapping(window)
    , m_currentNavigation(NavigationType::Trackball)
{
    std::cout << "Current navigation: Trackball" << std::endl;
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
        m_worldNavigation.reset(new WorldInHandNavigation(*cameraCapability, *viewportCapability, *m_coordProvider));
        m_trackballNavigation.reset(new TrackballNavigation(cameraCapability, viewportCapability));
    }
}

void QtViewerMapping::mapEvent(AbstractEvent * event)
{
    if (event->sourceType() == SourceType::Keyboard)
    {
        auto keyEvent = dynamic_cast<KeyboardEvent*>(event);
        assert(keyEvent != nullptr);

        if (keyEvent->type() == KeyboardEvent::Type::Press && keyEvent->key() == Key::KeyN)
        {
            if (m_currentNavigation == NavigationType::WorldInHand)
            {
                m_currentNavigation = NavigationType::Trackball;
                std::cout << "Current navigation: Trackball" << std::endl;
            }
            else
            {
                m_currentNavigation = NavigationType::WorldInHand;
                std::cout << "Current navigation: WorldInHand" << std::endl;
            }
        }
    }

    if (m_currentNavigation == NavigationType::WorldInHand)
    {
        mapEventWorld(event);
    }
    else
    {
        mapEventTrackball(event);
    }
}

void QtViewerMapping::mapEventTrackball(AbstractEvent * event)
{
    if (event->sourceType() == SourceType::Mouse)
    {
        MouseEvent * mouseEvent = dynamic_cast<MouseEvent*>(event);
        assert(mouseEvent != nullptr);

        switch (mouseEvent->type())
        {
        case MouseEvent::Type::Press:
            switch (mouseEvent->button())
            {
            case MouseButtonLeft:
                m_trackballNavigation->panBegin(mouseEvent->pos());
                break;
            case MouseButtonRight:
                m_trackballNavigation->rotateBegin(mouseEvent->pos());
                break;
            }
            break;

        case MouseEvent::Type::Move:
            switch (m_trackballNavigation->mode())
            {
            case TrackballNavigation::Mode::PAN:
                m_trackballNavigation->pan(mouseEvent->pos());
                break;
            case TrackballNavigation::Mode::ROTATE:
                m_trackballNavigation->rotate(mouseEvent->pos());
                break;
            }
            break;

        case MouseEvent::Type::Release:
            switch (m_trackballNavigation->mode())
            {
            case TrackballNavigation::Mode::PAN:
                m_trackballNavigation->panEnd();
                break;
            case TrackballNavigation::Mode::ROTATE:
                m_trackballNavigation->rotateEnd();
                break;
            }
            break;
        }
    }

    if (event->sourceType() == SourceType::Wheel)
    {
        auto wheelEvent = dynamic_cast<WheelEvent*>(event);
        assert(wheelEvent != nullptr);

        auto delta = wheelEvent->angleDelta().y;
        delta /= WheelEvent::defaultMouseAngleDelta();
        delta *= 0.1f; // smoother (slower) scaling
        m_trackballNavigation->zoom(delta);
    }
}

void QtViewerMapping::mapEventWorld(AbstractEvent * event)
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
                m_worldNavigation->pan(glm::vec3(0, 0, 1));
                break;
            case KeyA:
                m_worldNavigation->pan(glm::vec3(1, 0, 0));
                break;
            case KeyS:
                m_worldNavigation->pan(glm::vec3(0, 0, -1));
                break;
            case KeyD:
                m_worldNavigation->pan(glm::vec3(-1, 0, 0));
                break;
            // Reset camera position
            case KeyR:
                m_worldNavigation->reset();
                break;
            // Arrows rotate camera
            case KeyUp:
                m_worldNavigation->rotate(0.0f, glm::radians(-10.0f));
                break;
            case KeyLeft:
                m_worldNavigation->rotate(glm::radians(10.0f), 0.0f);
                break;
            case KeyDown:
                m_worldNavigation->rotate(0.0f, glm::radians(10.0f));
                break;
            case KeyRight:
                m_worldNavigation->rotate(glm::radians(-10.0f), 0.0f);
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
                m_worldNavigation->reset();
                break;
            case MouseButtonLeft:
                m_worldNavigation->panBegin(mouseEvent->pos());
                break;
            case MouseButtonRight:
                m_worldNavigation->rotateBegin(mouseEvent->pos());
                break;
            default:
                break;
            }
        }
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
        {
            switch (m_worldNavigation->mode())
            {
            case WorldInHandNavigation::InteractionMode::PanInteraction:
                m_worldNavigation->panProcess(mouseEvent->pos());
                break;
            case WorldInHandNavigation::InteractionMode::RotateInteraction:
                m_worldNavigation->rotateProcess(mouseEvent->pos());
                break;
            default:
                break;
            }
        }
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Release)
        {
            switch (mouseEvent->button())
            {
            case MouseButtonLeft:
                m_worldNavigation->panEnd();
                break;
            case MouseButtonRight:
                m_worldNavigation->rotateEnd();
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
            m_worldNavigation->scaleAtMouse(wheelEvent->pos(), scale);
        }
    }
}

