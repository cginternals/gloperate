
#include "gloperate-qt/viewer/QtOpenGLWindow.h"

#include <gloperate/ext-includes-begin.h>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QWindow>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractInputCapability.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/tools/ImageExporter.h>

#include <gloperate-qt/viewer/QtEventTransformer.h>
#include <gloperate-qt/scripting/TimerApi.h>


using namespace gloperate;


namespace gloperate_qt
{


QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager)
: m_resourceManager(resourceManager)
, m_painter(nullptr)
, m_timePropagator(nullptr)
, m_timerApi(nullptr)
{
}

QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager, const QSurfaceFormat & format)
: QtOpenGLWindowBase(format)
, m_resourceManager(resourceManager)
, m_painter(nullptr)
, m_timePropagator(nullptr)
, m_timerApi(nullptr)
{
}

QtOpenGLWindow::~QtOpenGLWindow()
{
}

Painter * QtOpenGLWindow::painter() const
{
    return m_painter;
}

void QtOpenGLWindow::setPainter(Painter * painter)
{
    // Save painter
    m_painter = painter;

    // Destroy old time propagator
    m_timePropagator = nullptr;

    if (!m_painter)
        return;

    m_timePropagator = make_unique<TimePropagator>(this);

    // Check for virtual time capability
    if (m_painter->supports<AbstractVirtualTimeCapability>())
        m_timePropagator->setCapability(m_painter->getCapability<AbstractVirtualTimeCapability>());

    m_initialized = false;
}

void QtOpenGLWindow::setTimerApi(TimerApi * timerApi)
{
    m_timerApi = timerApi;
}

void QtOpenGLWindow::onInitialize()
{
    // Initialize globjects
    globjects::init();
#ifndef N_DEBUG
    globjects::DebugMessage::enable(true);
#endif

    // Initialize painter
    if (m_painter)
    {
        AbstractViewportCapability * viewportCapability = m_painter->getCapability<AbstractViewportCapability>();

        if (viewportCapability)
        {
            // Resize painter
            qreal factor = QWindow::devicePixelRatio();
            viewportCapability->setViewport(0, 0, (int)(factor * width()), (int)(factor * height()));
        }

        m_painter->initialize();
    }
}

void QtOpenGLWindow::onResize(QResizeEvent * event)
{
    if (m_painter)
    {
        // Check if the painter supports the viewport capability
        AbstractViewportCapability * viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
        if (viewportCapability)
        {
            // Resize painter
            viewportCapability->setViewport(0, 0, event->size().width(), event->size().height());
        }
    }
}

void QtOpenGLWindow::onPaint()
{
    // Update script timers
    if (m_timerApi && m_painter) {
        AbstractVirtualTimeCapability * virtualTimeCapability = m_painter->getCapability<AbstractVirtualTimeCapability>();
        if (virtualTimeCapability) {
            m_timerApi->tickUpdate(virtualTimeCapability->delta());
        }
    }

    if (m_painter) {
        // Call painter
        m_painter->paint();
    }
    else
    {
        qreal factor = QWindow::devicePixelRatio();

        gl::glClearColor(1.0, 1.0, 1.0, 1.0);
        gl::glViewport(0, 0, (int)(factor * width()), (int)(factor * height()));
        gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    }
}

void QtOpenGLWindow::keyPressEvent(QKeyEvent * event)
{
    makeCurrent();

    if (event->key() == Qt::Key_F10)
    {
        if (ImageExporter::isApplicableTo(painter()))
        {
			ImageExporter image(painter(), m_resourceManager);
            image.initialize();
            image.save("image.png");
        }
    }

    doneCurrent();

    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onKeyDown(
            QtEventTransformer::fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void QtOpenGLWindow::keyReleaseEvent(QKeyEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onKeyUp(
            QtEventTransformer::fromQtKeyCode(event->key(), event->modifiers())
        );
    }
}

void QtOpenGLWindow::mouseMoveEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseMove(
            (int)(event->x() * devicePixelRatio()),
            (int)(event->y() * devicePixelRatio())
        );
    }
}

void QtOpenGLWindow::mousePressEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMousePress(
            (int)(event->x() * devicePixelRatio()),
            (int)(event->y() * devicePixelRatio()),
            QtEventTransformer::fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::mouseReleaseEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseRelease(
            (int)(event->x() * devicePixelRatio()),
            (int)(event->y() * devicePixelRatio()),
            QtEventTransformer::fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::mouseDoubleClickEvent(QMouseEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseDoubleClick(
            (int)(event->x() * devicePixelRatio()),
            (int)(event->y() * devicePixelRatio()),
            QtEventTransformer::fromQtMouseButton(event->button())
        );
    }
}

void QtOpenGLWindow::wheelEvent(QWheelEvent * event)
{
    // Check for input capability
    if (m_painter && m_painter->supports<gloperate::AbstractInputCapability>())
    {
        // Propagate event
        m_painter->getCapability<gloperate::AbstractInputCapability>()->onMouseWheel(
            event->orientation() == Qt::Vertical ? 0 : event->delta(),
            event->orientation() == Qt::Vertical ? event->delta() : 0
        );
    }
}


} // namespace gloperate_qt
