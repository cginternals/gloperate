
#include <gloperate-qtquick/RenderItem.h>

#include <QQmlContext>
#include <QQuickWindow>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/Stage.h>

#include <gloperate-qt/base/input.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/Utils.h>

#include <gloperate-qtquick/RenderItemRenderer.h>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem(QQuickItem * parent)
: QQuickFramebufferObject(parent)
, m_stage("")
{
    // Set input modes
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    // Connect signal to update the frame
    connect(
        this, &RenderItem::updateNeeded,
        this, &RenderItem::onUpdate,
        Qt::QueuedConnection
    );
}

RenderItem::~RenderItem()
{
}

const QString & RenderItem::stage() const
{
    // Return name of render stage
    return m_stage;
}

void RenderItem::setStage(const QString & name)
{
    if (m_canvas)
    {
        // Create and update render stage
        // Stage is saved internally
        updateStage(name);
    }
    else
    {
        // Create canvas with render stage
        // Stage is saved internally
        createCanvasWithStage(name);
    }
}

const std::shared_ptr<gloperate::AbstractCanvas> & RenderItem::canvas() const
{
    return m_canvas;
}

QQuickFramebufferObject::Renderer * RenderItem::createRenderer() const
{
    return new RenderItemRenderer(const_cast<RenderItem *>(this));
}

void RenderItem::onUpdate()
{
    // Schedule item redraw
    update();
}

void RenderItem::createCanvasWithStage(const QString & stage)
{
    // Get gloperate environment
    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());
    gloperate::Environment * environment = engine->environment();

    // Create stage before
    auto stageInstance = Utils::createRenderStage(environment, m_stage.toStdString());

    if (!stageInstance)
    {
        return;
    }

    // Create canvas and render stage
    m_stage = stage;
    m_canvas = Utils::createCanvas(
        environment,
        std::move(stageInstance)
    );

    assert(m_canvas);

    // Set viewport size
    auto ratio = window()->devicePixelRatio();
    m_canvas->onViewport(
        glm::vec4(0, 0, width() * ratio, height() * ratio)
      , glm::vec4(0, 0, width(), height())
    );

    // Repaint window when canvas needs to be updated
    m_canvas->redraw.connect([this] ()
    {
        emit updateNeeded();
    } );

    m_canvas->wakeup.connect([] ()
    {
        QCoreApplication::instance()->processEvents();
    } );

    // Inform about initialization of the canvas
    emit canvasInitialized();

    // Inform about replacement of the render stage
    emit renderStageReplaced();
}

void RenderItem::updateStage(const QString & stage)
{
    // Get gloperate environment
    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());
    gloperate::Environment * environment = engine->environment();

    // Create stage before
    auto stageInstance = Utils::createRenderStage(environment, stage.toStdString());

    if (!stageInstance)
    {
        return;
    }

    // Update render stage
    m_stage = stage;

    auto canvas = static_cast<gloperate::Canvas*>(m_canvas.get());

    window()->scheduleRenderJob(new RenderStageCleanup(canvas->obtainRenderStage(), canvas), QQuickWindow::BeforeRenderingStage);
    window()->scheduleRenderJob(new RenderStageInitialization(stageInstance.get(), canvas), QQuickWindow::BeforeRenderingStage);

    canvas->setRenderStage(std::move(stageInstance));

    // Inform about replacement of the render stage
    emit renderStageReplaced();
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
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
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
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
}

void RenderItem::mouseMoveEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMouseMove(glm::ivec2(
            (int)(event->x() * window()->devicePixelRatio()),
            (int)(event->y() * window()->devicePixelRatio()))
        );
    }
}

void RenderItem::mousePressEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMousePress(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
        );
    }
}

void RenderItem::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->onMouseRelease(
            fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
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
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
        );
    }
}

void RenderItem::releaseResources()
{
    if (m_canvas)
    {
        m_canvas->onContextDeinit();
    }
}


RenderItem::RenderStageInitialization::RenderStageInitialization(gloperate::Stage * stage, gloperate::Canvas * canvas)
: m_canvas(canvas)
, m_stage(stage)
{
}

void RenderItem::RenderStageInitialization::run()
{
    m_stage->initContext(m_canvas->openGLContext());
}


RenderItem::RenderStageCleanup::RenderStageCleanup(std::unique_ptr<gloperate::Stage> && stage, gloperate::Canvas * canvas)
: m_canvas(canvas)
, m_stage(std::move(stage))
{
}

void RenderItem::RenderStageCleanup::run()
{
    m_stage->deinitContext(m_canvas->openGLContext());
}


} // namespace gloperate_qtquick
