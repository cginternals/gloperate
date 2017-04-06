
#include <gloperate-qtquick/RenderItem.h>

#include <iostream>

#include <QVariant>
#include <QColor>
#include <QQmlContext>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>

#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/pipeline/Stage.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/input.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/QuickView.h>
#include <gloperate-qtquick/Utils.h>

#include <gloperate-qtquick/RenderItemRenderer.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem(QQuickItem * parent)
: QQuickFramebufferObject(parent)
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

    connect(
        this, &RenderItem::updated,
        this, &RenderItem::doUpdate,
        Qt::QueuedConnection
    );
}

RenderItem::~RenderItem()
{
}

const gloperate::AbstractCanvas * RenderItem::canvas() const
{
    return m_canvas.get();
}

gloperate::AbstractCanvas * RenderItem::canvas()
{
    return m_canvas.get();
}

QQuickFramebufferObject::Renderer * RenderItem::createRenderer() const
{
    return new RenderItemRenderer(const_cast<RenderItem *>(this));
}

const QString & RenderItem::stage() const
{
    return m_stage;
}

void RenderItem::setStage(const QString & name)
{
    m_stage = name;

std::cout << "Create stage " << m_stage.toStdString() << std::endl;

    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());

    // Create canvas and render stage
    gloperate::Environment * environment = engine->environment();

    m_canvas = Utils::createCanvas(
        environment,
        Utils::createRenderStage(environment, m_stage.toStdString())
    );

    assert(m_canvas);

    // Repaint window when canvas needs to be updated
    m_canvas->redraw.connect([this] ()
    {
        emit this->updated();
    } );

    m_canvas->wakeup.connect([] ()
    {
        QCoreApplication::instance()->processEvents();
    } );

    emit canvasInitialized();
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

std::cout << "Create stage " << m_stage.toStdString() << std::endl;

    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());

    // Create canvas and render stage
    gloperate::Environment * environment = engine->environment();

    m_canvas = Utils::createCanvas(
        environment,
        Utils::createRenderStage(environment, m_stage.toStdString())
    );

    assert(m_canvas);

    // Repaint window when canvas needs to be updated
    m_canvas->redraw.connect([this] ()
    {
        emit this->updated();
    } );

    m_canvas->wakeup.connect([] ()
    {
        QCoreApplication::instance()->processEvents();
    } );

    emit canvasInitialized();
}

void RenderItem::doRender(int fboId)
{
std::cout << "doRender1" << std::endl;

    if (!m_canvas || !this->window())
    {
        return;
    }

std::cout << "doRender2" << std::endl;

    // Get qml window
    auto * window = this->window();

    // Initialize canvas before rendering the first time
    if (!m_initialized)
    {
        m_canvas->setOpenGLContext(m_context.get());

        m_initialized = true;
    }

    /*
    // Get background color
    if (window->rootObject())
    {
        QVariant var = window->rootObject()->property("backgroundColor");
        QColor color = var.value<QColor>();
        m_canvas->onBackgroundColor(color.redF(), color.greenF(), color.blueF());
    }
    */

    // [TODO]: optimize memory reallocation problem
    auto defaultFBO = globjects::Framebuffer::fromId(fboId);

    m_canvas->onViewport(
        glm::vec4(0, 0, 400, 400)
      , glm::vec4(0, 0, 400, 400)
    );

std::cout << "doRender3" << std::endl;

    m_canvas->render(defaultFBO.get());

    // Reset OpenGL state
    globjects::VertexArray::unbind();
    window->resetOpenGLState();
}

void RenderItem::doUpdate()
{
    std::cout << "doUpdate()" << std::endl;

    this->update();
}

void RenderItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
    /*
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
    */
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
    /*
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    if (m_canvas)
    {
        m_canvas->onKeyPress(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
        );
    }
    */
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
    /*
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    if (m_canvas)
    {
        m_canvas->onKeyRelease(
            fromQtKeyCode(event->key(), event->modifiers()),
            fromQtModifiers(event->modifiers())
        );
    }
    */
}

void RenderItem::mouseMoveEvent(QMouseEvent * event)
{
    /*
    if (m_canvas)
    {
        m_canvas->onMouseMove(glm::ivec2(
            (int)(event->x() * m_devicePixelRatio),
            (int)(event->y() * m_devicePixelRatio))
        );
    }
    */
}

void RenderItem::mousePressEvent(QMouseEvent * event)
{
    /*
    if (m_canvas)
    {
        m_canvas->onMousePress(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
    */
}

void RenderItem::mouseReleaseEvent(QMouseEvent * event)
{
    /*
    if (m_canvas)
    {
        m_canvas->onMouseRelease(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
    */
}

void RenderItem::wheelEvent(QWheelEvent * event)
{
    /*
    if (m_canvas)
    {
        m_canvas->onMouseWheel(
            glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                       event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
            glm::ivec2( (int)(event->x() * m_devicePixelRatio),
                        (int)(event->y() * m_devicePixelRatio) )
        );
    }
    */
}


} // namespace gloperate_qtquick
