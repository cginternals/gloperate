
#include <gloperate-qtquick/base/RenderItem.h>

#include <QVariant>
#include <QColor>

#include <gloperate/base/AbstractCanvas.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/input.h>

#include <gloperate-qtquick/base/QuickView.h>
#include <gloperate-qtquick/base/Utils.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem(QQuickItem * parent)
: QQuickItem(parent)
, m_canvas(nullptr)
, m_devicePixelRatio(1.0f)
, m_initialized(false)
, m_stage("")
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

gloperate::AbstractCanvas * RenderItem::canvas() const
{
    return m_canvas;
}

QString RenderItem::stage() const
{
    return m_stage;
}

void RenderItem::setStage(const QString & name)
{
    m_stage = name;
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

    // Create canvas and render stage
    QuickView * view = static_cast<QuickView*>(window);
    if (view)
    {
        m_canvas = Utils::createCanvas(
            view->environment(),
            Utils::createRenderStage(view->environment(), m_stage.toStdString())
        );
    }

    // Repaint window when canvas needs to be updated
    m_canvas->redraw.connect([this] ()
    {
        if (this->window())
        {
            this->window()->update();
        }
    } );

    m_canvas->wakeup.connect([] ()
    {
        QCoreApplication::instance()->processEvents();
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
    if (!m_canvas || !this->window())
    {
        return;
    }

    // Get qml view
    QuickView * view = static_cast<QuickView*>(this->window());

    // Initialize canvas before rendering the first time
    if (!m_initialized)
    {
        m_canvas->setOpenGLContext(view->context());

        m_initialized = true;
    }

    // Get background color    
    if (view->rootObject())
    {
        QVariant var = view->rootObject()->property("backgroundColor");
        QColor color = var.value<QColor>();
        m_canvas->onBackgroundColor(color.redF(), color.greenF(), color.blueF());
    }

    // Render into item
    m_canvas->onRender();

    // Reset OpenGL state
    window()->resetOpenGLState();
}

void RenderItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (!m_canvas)
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

    m_canvas->onViewport(
        glm::vec4(devX,  devY,  devWidth,  devHeight)
      , glm::vec4(virtX, virtY, virtWidth, virtHeight)
    );
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onKeyPress(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
        );
    }
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onKeyRelease(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
        );
    }
}

void RenderItem::mouseMoveEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMouseMove(glm::ivec2(
            (int)(event->x() * m_devicePixelRatio),
            (int)(event->y() * m_devicePixelRatio))
        );
    }
}

void RenderItem::mousePressEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMousePress(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}

void RenderItem::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMouseRelease(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}

void RenderItem::wheelEvent(QWheelEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMouseWheel(
            glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                       event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
}


} // namespace gloperate_qtquick
