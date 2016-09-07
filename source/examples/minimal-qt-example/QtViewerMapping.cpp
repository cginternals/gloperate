
#include "QtViewerMapping.h"

#include <iostream>

#include <glbinding/gl/enum.h>

#include <gloperate/base/RenderTargetType.h>
#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/Camera.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/WheelEvent.h>
#include <gloperate/navigation/WorldInHandNavigation.h>
#include <gloperate/navigation/TrackballNavigation.h>
#include <gloperate/navigation/CoordinateProvider.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate;
using namespace gloperate_qt;
using gloperate::make_unique;


QtViewerMapping::QtViewerMapping(QtOpenGLWindow * window)
:   AbstractQtMapping(window)
,   m_currentNavigation(NavigationType::WorldInHand)
{
}

QtViewerMapping::~QtViewerMapping()
{
}

void QtViewerMapping::initializeTools()
{
    m_renderTarget = nullptr;

    if (m_painter && 
        m_painter->supports<AbstractCameraCapability>() &&
        m_painter->supports<AbstractViewportCapability>() &&
        m_painter->supports<AbstractProjectionCapability>() &&
        (m_painter->supports<AbstractTypedRenderTargetCapability>() ||
         m_painter->supports<AbstractTargetFramebufferCapability>()))
    {
        auto cameraCapability = m_painter->getCapability<AbstractCameraCapability>();
        auto projectionCapability = m_painter->getCapability<AbstractProjectionCapability>();
        auto viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
        
        auto renderTargetCapability = m_painter->getCapability<AbstractTypedRenderTargetCapability>();
        if (!renderTargetCapability)
        {
            m_renderTarget = make_unique<TypedRenderTargetCapability>();
            renderTargetCapability = m_renderTarget.get();

            auto fboCapability = m_painter->getCapability<AbstractTargetFramebufferCapability>();
            fboCapability->changed.connect([this] () { this->onTargetFramebufferChanged(); });
        }

        m_coordProvider = make_unique<CoordinateProvider>(
            cameraCapability, projectionCapability, viewportCapability, renderTargetCapability);
        m_worldNavigation = make_unique<WorldInHandNavigation>(
            *cameraCapability, *viewportCapability, *m_coordProvider);
        m_trackballNavigation = make_unique<TrackballNavigation>(cameraCapability, viewportCapability);
    }
}

void QtViewerMapping::mapEvent(AbstractEvent * event)
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

void QtViewerMapping::mapKeyboardEvent(KeyboardEvent * event)
{
    if (event->type() == KeyboardEvent::Type::Press && event->key() == Key::KeyN)
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

    if (event && event->type() == KeyboardEvent::Type::Press)
    {
        switch (m_currentNavigation)
        {
        case NavigationType::WorldInHand:
        switch (event->key())
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
            break;

        default:
            break;
    }
}
}

void QtViewerMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    switch (m_currentNavigation)
    {
    case QtViewerMapping::NavigationType::WorldInHand:
    if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Press)
    {
        const auto pos = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());

        switch (mouseEvent->button())
        {
        case MouseButtonMiddle:
                m_worldNavigation->reset();
            break;
        case MouseButtonLeft:
                m_worldNavigation->panBegin(pos);
            break;
        case MouseButtonRight:
                m_worldNavigation->rotateBegin(pos);
            break;
        default:
            break;
        }
    }
    else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
    {
        const auto pos = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
        
            switch (m_worldNavigation->mode())
        {
        case WorldInHandNavigation::InteractionMode::PanInteraction:
                m_worldNavigation->panProcess(pos);
            break;
        case WorldInHandNavigation::InteractionMode::RotateInteraction:
                m_worldNavigation->rotateProcess(pos);
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
        break;

    case QtViewerMapping::NavigationType::Trackball:
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

        break;
    default:
        break;
    }
}

void QtViewerMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    auto scale = wheelEvent->angleDelta().y;
    scale /= WheelEvent::defaultMouseAngleDelta();
    scale *= 0.1f; // smoother (slower) scaling

    switch (m_currentNavigation) {
    case NavigationType::WorldInHand:
        m_worldNavigation->scaleAtMouse(wheelEvent->pos(), scale);
        break;
    case NavigationType::Trackball:
        m_trackballNavigation->zoom(scale);
        break;
    }
}

void QtViewerMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    
    if (!fbo)
        fbo = globjects::Framebuffer::defaultFBO();

    m_renderTarget->setRenderTarget(gloperate::RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}
