
#include <gloperate-qt/viewer/DefaultMapping.h>

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


using namespace gloperate_qt;
using namespace gloperate;

using gloperate::make_unique;


namespace
{
    const int g_tooltipTimeout = 200;
}


namespace gloperate_qt
{


DefaultMapping::DefaultMapping(QtOpenGLWindow * window)
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

DefaultMapping::~DefaultMapping()
{
}

void DefaultMapping::initializeTools()
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

        m_coordProvider = make_unique<CoordinateProvider>(
            cameraCapability, projectionCapability, m_viewportCapability, m_typedRenderTargetCapability);
        m_navigation = make_unique<WorldInHandNavigation>(
            *cameraCapability, *m_viewportCapability, *m_coordProvider);
    }

    m_metaInformationCapability = m_painter->getCapability<AbstractMetaInformationCapability>();
}

void DefaultMapping::mapEvent(AbstractEvent * event)
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

void DefaultMapping::mapKeyboardEvent(KeyboardEvent * event)
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

void DefaultMapping::mapMouseEvent(MouseEvent * mouseEvent)
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
        case WorldInHandNavigation::InteractionMode::PanInteraction:
            m_navigation->panProcess(m_currentMousePosition);
            break;
        case WorldInHandNavigation::InteractionMode::RotateInteraction:
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

void DefaultMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    auto scale = wheelEvent->angleDelta().y;
    scale /= WheelEvent::defaultMouseAngleDelta();
    scale *= 0.1f; // smoother (slower) scaling
    m_navigation->scaleAtMouse(wheelEvent->pos(), scale);
}

void DefaultMapping::onTargetFramebufferChanged()
{
    auto fbo = m_painter->getCapability<AbstractTargetFramebufferCapability>()->framebuffer();
    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    m_renderTarget->setRenderTarget(gloperate::RenderTargetType::Depth, fbo,
        gl::GL_DEPTH_ATTACHMENT, gl::GL_DEPTH_COMPONENT);
}

void DefaultMapping::showTooltip()
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

void DefaultMapping::hideTooltip()
{
    QToolTip::showText(QPoint(0, 0), "");
}


} // namespace gloperate_qt
