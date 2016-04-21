
#include <gloperate-qt/viewer/TreeNavigationMapping.h>
#include <gloperate-qt/viewer/TreeNavigation.h>

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
    const int g_tooltipTimeout = 200;
    const float SCALE_FACTOR = 0.01f;
}

namespace gloperate_qt{

TreeNavigationMapping::TreeNavigationMapping(QtOpenGLWindow * window)
: AbstractQtMapping(window)
, m_metaInformationCapability(nullptr)
, m_viewportCapability(nullptr)
, m_typedRenderTargetCapability(nullptr)
, m_timer(new QTimer(this))
{
    m_timer->setInterval(g_tooltipTimeout);
    m_timer->setSingleShot(true);
    m_timer->stop();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(showTooltip()));
}

TreeNavigationMapping::~TreeNavigationMapping()
{
}

void TreeNavigationMapping::initializeTools()
{
    m_renderTarget = nullptr;
    m_metaInformationCapability = nullptr;
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
        m_navigation = make_unique<TreeNavigation>(
            *cameraCapability, *m_viewportCapability, *m_coordProvider, projectionCapability);
    }

    m_metaInformationCapability = m_painter->getCapability<AbstractMetaInformationCapability>();
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
            break;
        case MouseButtonRight:
            m_navigation->rotateBegin(m_currentMousePosition);
            break;
        default:
            break;
        }
    }
    else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
    {
        if (m_metaInformationCapability)
        {
            hideTooltip();
            m_timer->start();
        }

        switch (m_navigation->mode())
        {
        case TreeNavigation::InteractionMode::PanInteraction:
            m_navigation->panProcess(m_currentMousePosition);
            break;
        case TreeNavigation::InteractionMode::RotateInteraction:
            m_navigation->rotateProcess(m_currentMousePosition);
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

void TreeNavigationMapping::showTooltip()
{
    if (!m_metaInformationCapability)
    {
        return;
    }

    hideTooltip();

    m_window->makeCurrent();
    int id = gloperate::ObjectIdExtractor(m_viewportCapability, m_typedRenderTargetCapability).get(m_currentMousePosition);
    m_window->doneCurrent();

    const std::string & string = m_metaInformationCapability->get(id);

    if (string.empty())
    {
        return;
    }

    QToolTip::showText(m_window->mapToGlobal(QPoint(m_currentMousePosition.x, m_currentMousePosition.y)), QString::fromStdString(string));
}

void TreeNavigationMapping::hideTooltip()
{
    QToolTip::showText(QPoint(0, 0), "");
}

}
