
#include <gloperate-qtquick/RenderItem.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/Surface.h>

#include <gloperate-qt/viewer/input.h>

#include <gloperate-qtquick/QuickView.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem()
: m_surface(nullptr)
, m_devicePixelRatio(1.0f)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);

    // Connect to event when the window into which is rendered has changed
    connect(
        this, &QQuickItem::windowChanged,
        this, &RenderItem::onWindowChanged,
        Qt::DirectConnection
    );
}

RenderItem::~RenderItem()
{
}

void RenderItem::onWindowChanged(QQuickWindow * window)
{
    // Check if window is valid
    if (!window)
    {
        return;
    }

    // Get device/pixel-ratio
    m_devicePixelRatio = window->devicePixelRatio();

    // Get surface from window
    QuickView * view = static_cast<QuickView*>(window);
    if (view)
    {
        m_surface = view->surface();
    }

    // Do not clear surface when rendering Qml, because we render our content first
    window->setClearBeforeRendering(false);

    // Connect to window draw event
    connect(
        window, &QQuickWindow::beforeRendering,
        this, &RenderItem::onBeforeRendering,
        Qt::DirectConnection
    );
}

void RenderItem::onBeforeRendering()
{
    if (!m_surface)
    {
        return;
    }

    m_surface->onRender();
}

void RenderItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (!m_surface)
    {
        return;
    }

    float width  = newGeometry.width();
    float height = newGeometry.height();

    m_surface->onResize(
        glm::ivec2(width * m_devicePixelRatio, height * m_devicePixelRatio),
        glm::ivec2(width, height)
    );
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
    if (m_surface)
    {
        m_surface->onKeyPress(
            fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
    if (m_surface)
    {
        m_surface->onKeyRelease(
            fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void RenderItem::mouseMoveEvent(QMouseEvent * event)
{
    if (m_surface)
    {
        m_surface->onMouseMove(glm::ivec2(
            (int)(event->x() * m_devicePixelRatio),
            (int)(event->y() * m_devicePixelRatio))
        );
    }
}

void RenderItem::mousePressEvent(QMouseEvent * event)
{
    if (m_surface)
    {
        m_surface->onMousePress(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}

void RenderItem::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_surface)
    {
        m_surface->onMouseRelease(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}

void RenderItem::wheelEvent(QWheelEvent * event)
{
    if (m_surface)
    {
        m_surface->onMouseWheel(
            glm::ivec2( event->orientation() == Qt::Vertical ? 0 : event->delta(),
                        event->orientation() == Qt::Vertical ? event->delta() : 0 ),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}


} // namespace gloperate_qtquick
