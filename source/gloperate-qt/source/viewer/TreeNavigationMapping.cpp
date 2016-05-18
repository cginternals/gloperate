
#include <gloperate-qt/viewer/TreeNavigationMapping.h>
#include <gloperate/navigation/TreeMapNavigation.h>

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
#include <gloperate/navigation/CoordinateProvider.h>
#include <gloperate/tools/ObjectIdExtractor.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate_qt;
using namespace gloperate;

using gloperate::make_unique;


namespace
{
    const float SCALE_FACTOR = 0.1f;
}

namespace gloperate_qt{

TreeNavigationMapping::TreeNavigationMapping(QtOpenGLWindow * window)
: AbstractQtMapping(window)
, m_viewportCapability(nullptr)
, m_typedRenderTargetCapability(nullptr)
{
}

TreeNavigationMapping::~TreeNavigationMapping()
{
}

void TreeNavigationMapping::initializeTools()
{
    m_renderTarget = nullptr;
    m_viewportCapability = nullptr;
    m_typedRenderTargetCapability = nullptr;
    m_coordProvider = nullptr;
    m_navigation = nullptr;

    if (!m_painter)
    {
        return;
    }
    
    bool supportsCameraViewProjection = (m_painter->supports<AbstractCameraCapability>() &&
                                          m_painter->supports<AbstractViewportCapability>() &&
                                          m_painter->supports<AbstractProjectionCapability>());
                                          
    bool supportsRenderTarget = (m_painter->supports<AbstractTypedRenderTargetCapability>() ||
                                 m_painter->supports<AbstractTargetFramebufferCapability>());

    if (supportsRenderTarget && supportsCameraViewProjection)
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

        m_coordProvider = make_unique<CoordinateProvider>(
            cameraCapability, projectionCapability, m_viewportCapability, m_typedRenderTargetCapability);
        m_navigation = make_unique<gloperate::TreeMapNavigation>(
            *cameraCapability, *m_viewportCapability, *m_coordProvider, projectionCapability);
    }
}

void TreeNavigationMapping::mapEvent(AbstractEvent * event)
{
    if (!m_navigation)
    {
        return;
    }

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

void TreeNavigationMapping::mapKeyboardEvent(KeyboardEvent * event)
{
    // No Keyboard Interaction as of yet, as the old one is choppy
}

void TreeNavigationMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    if (mouseEvent)
    {
        m_currentMousePosition = mouseEvent->pos() * static_cast<int>(m_window->devicePixelRatio());
    }

    if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Press)
    {
        switch (mouseEvent->button())
        {
        case MouseButtonMiddle:
            m_navigation->reset();
            break;
        case MouseButtonLeft:
            m_navigation->panBegin(m_currentMousePosition);
            m_interactionMode = InteractionMode::PanInteraction;
            break;
        case MouseButtonRight:
            m_navigation->rotateBegin(m_currentMousePosition);
            m_interactionMode = InteractionMode::RotateInteraction;
            break;
        default:
            break;
        }
    }
    else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
    {
        switch (m_interactionMode)
        {
        case InteractionMode::PanInteraction:
            m_navigation->panProcess(m_currentMousePosition);
            break;
        case InteractionMode::RotateInteraction:
            m_navigation->rotateProcess(m_currentMousePosition);
            break;
        default:
            break;
        }
    }
    else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Release)
    {
        m_interactionMode = InteractionMode::NoInteraction;
    }
}

void TreeNavigationMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    auto scale = wheelEvent->angleDelta().y;
    scale /= WheelEvent::defaultMouseAngleDelta();
    scale *= SCALE_FACTOR; // smoother (slower) scaling
    m_navigation->scaleAtMouse(wheelEvent->pos(), scale);
}

void TreeNavigationMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    m_renderTarget->setRenderTarget(gloperate::RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}

} // namespace gloperate_qt
