
#include <gloperate-qtquick/viewer/RenderItem.h>

#include <QVariant>
#include <QColor>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/Surface.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/input.h>

#include <gloperate-qtquick/viewer/QuickView.h>
#include <gloperate-qtquick/viewer/Utils.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem(QQuickItem * parent)
: QQuickItem(parent)
, m_surface(nullptr)
, m_devicePixelRatio(1.0f)
, m_initialized(false)
{
    // Set input modes
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

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

gloperate::Surface * RenderItem::surface() const
{
    return m_surface;
}

void RenderItem::onWindowChanged(QQuickWindow * window)
{
    // Check if window is valid
    if (!window)
    {
        return;
    }

    // Get device/pixel-ratio
    m_devicePixelRatio = window->effectiveDevicePixelRatio();

    // Create render surface and render stage
    QuickView * view = static_cast<QuickView*>(window);
    if (view)
    {
        m_surface = Utils::createSurface(view->viewerContext(), Utils::createRenderStage(view->viewerContext()));
    }

    // Repaint window when surface needs to be updated
    m_surface->redrawNeeded.connect([this] ()
    {
        this->window()->update();
    } );

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

    // Initialize surface before rendering the first time
    if (!m_initialized)
    {
        QuickView * view = static_cast<QuickView*>(this->window());
        m_surface->setOpenGLContext(view->context());

        m_initialized = true;
    }

    // Get background color    
    QuickView * view = static_cast<QuickView*>(this->window());
    QVariant var = view->rootObject()->property("backgroundColor");
    QColor color = var.value<QColor>();
    m_surface->onBackgroundColor(color.redF(), color.greenF(), color.blueF());

    // Render into item
    m_surface->onRender();

    // Reset OpenGL state
    window()->resetOpenGLState();
}

void RenderItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (!m_surface)
    {
        return;
    }

    QPointF pos1 = parentItem()->mapToScene(newGeometry.topLeft());
    QPointF pos2 = parentItem()->mapToScene(newGeometry.bottomRight());

    float virtX      = pos1.x();
    float virtY      = pos1.y();
    float virtWidth  = pos2.x() - pos1.x() + 1;
    float virtHeight = pos2.y() - pos1.y() + 1;

    float screenHeight = window()->height();
    virtY = screenHeight - (virtY + virtHeight);

    float devX       = virtX      * m_devicePixelRatio;
    float devY       = virtY      * m_devicePixelRatio;
    float devWidth   = virtWidth  * m_devicePixelRatio;
    float devHeight  = virtHeight * m_devicePixelRatio;

    m_surface->onViewport(
        glm::ivec4(devX,  devY,  devWidth,  devHeight)
      , glm::ivec4(virtX, virtY, virtWidth, virtHeight)
    );
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
    if (m_surface)
    {
        m_surface->onKeyPress(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
        );
    }
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
    if (m_surface)
    {
        m_surface->onKeyRelease(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
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
            glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                       event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}

void RenderItem::onCreateVideo()
{
    m_surface->createVideo();
}


} // namespace gloperate_qtquick
