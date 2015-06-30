
#include "QtViewerMapping.h"

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
#include <gloperate/navigation/CoordinateProvider.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate;
using namespace gloperate_qt;
using gloperate::make_unique;


QtViewerMapping::QtViewerMapping(QtOpenGLWindow * window)
:   AbstractQtMapping(window)
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
        m_navigation = make_unique<WorldInHandNavigation>(
            *cameraCapability, *viewportCapability, *m_coordProvider);
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
    if (event && event->type() == KeyboardEvent::Type::Press)
    {
        switch (event->key())
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

void QtViewerMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Press)
    {
        const auto pos = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());

        switch (mouseEvent->button())
        {
        case MouseButtonMiddle:
            m_navigation->reset();
            break;
        case MouseButtonLeft:
            m_navigation->panBegin(pos);
            break;
        case MouseButtonRight:
            m_navigation->rotateBegin(pos);
            break;
        default:
            break;
        }
    }
    else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
    {
        const auto pos = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
        
        switch (m_navigation->mode())
        {
        case WorldInHandNavigation::InteractionMode::PanInteraction:
            m_navigation->panProcess(pos);
            break;
        case WorldInHandNavigation::InteractionMode::RotateInteraction:
            m_navigation->rotateProcess(pos);
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
            m_navigation->panEnd();
            break;
        case MouseButtonRight:
            m_navigation->rotateEnd();
            break;
        default:
            break;
        }
    }
}

void QtViewerMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    auto scale = wheelEvent->angleDelta().y;
    scale /= WheelEvent::defaultMouseAngleDelta();
    scale *= 0.1f; // smoother (slower) scaling
    m_navigation->scaleAtMouse(wheelEvent->pos(), scale);
}

void QtViewerMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    
    if (!fbo)
        fbo = globjects::Framebuffer::defaultFBO();

    m_renderTarget->setRenderTarget(gloperate::RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}
