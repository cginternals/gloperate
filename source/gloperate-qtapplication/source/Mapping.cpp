#include <gloperate-qtapplication/Mapping.h>

#include <QDebug>
#include <QToolTip>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>

#include <gloperate/painter/Camera.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>
#include <gloperate/painter/AbstractMetaInformationCapability.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/WheelEvent.h>
#include <gloperate/navigation/WorldInHandNavigation.h>
#include <gloperate/tools/CoordinateProvider.h>
#include <gloperate/tools/ObjectIdExtractor.h>

#include <gloperate-qt/QtOpenGLWindow.h>


using namespace gloperate;
using namespace gloperate_qt;

namespace
{
    const int tooltipTimeout = 200;
}

namespace gloperate_qtapplication
{

Mapping::Mapping(QtOpenGLWindow * window)
    : AbstractQtMapping(window)
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(tooltipTimeout);
    m_timer->setSingleShot(true);
    m_timer->stop();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(showTooltip()));
}

Mapping::~Mapping()
{
}

void Mapping::initializeTools()
{
    m_metaInformationCapability = m_painter->getCapability<AbstractMetaInformationCapability>();
    m_typedRenderTargetCapability = m_painter->getCapability<AbstractTypedRenderTargetCapability>();
    m_viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
    if (    m_painter &&
            m_painter->supports<AbstractCameraCapability>() &&
            m_viewportCapability &&
            m_typedRenderTargetCapability &&
            m_painter->supports<AbstractPerspectiveProjectionCapability>())
    {
        AbstractCameraCapability * cameraCapability = m_painter->getCapability<AbstractCameraCapability>();
        AbstractPerspectiveProjectionCapability * projectionCapability = m_painter->getCapability<AbstractPerspectiveProjectionCapability>();

        m_coordProvider.reset(new CoordinateProvider(cameraCapability, projectionCapability, m_viewportCapability, m_typedRenderTargetCapability));
        m_navigation.reset(new WorldInHandNavigation(*cameraCapability, *m_viewportCapability, *m_coordProvider));

        float h = 0.5/tan(glm::radians(40.0)/2.0);
        cameraCapability->setCenter(glm::vec3(0.0, 0.0, 0.0));
        cameraCapability->setEye(glm::vec3(0.0, h, 1.0));
        cameraCapability->setUp(glm::vec3(0.0, 1.0, 0.0));
        projectionCapability->setZNear(0.01f);
        projectionCapability->setZFar(10.0f);
    }
    else
    {
        m_coordProvider.reset(nullptr);
        m_navigation.reset(nullptr);
    }
}

void Mapping::mapEvent(AbstractEvent * event)
{
    if (!m_navigation)
    {
        return;
    }

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
        if (mouseEvent)
        {
            m_currentMousePosition = mouseEvent->pos();
        }

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
            case MouseButtonRight:
                m_navigation->rotateBegin(mouseEvent->pos());
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
                m_navigation->panProcess(mouseEvent->pos());
                break;
            case WorldInHandNavigation::InteractionMode::RotateInteraction:
                m_navigation->rotateProcess(mouseEvent->pos());
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
        else if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Leave)
        {
            if (m_metaInformationCapability)
            {
                hideTooltip();
                m_timer->stop();
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

void Mapping::showTooltip()
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

void Mapping::hideTooltip()
{
    QToolTip::showText(QPoint(0, 0), "");
}

} // namespace gloperate_qtapplication
