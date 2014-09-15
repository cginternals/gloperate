/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include "gloperate-qt/QtOpenGLWindow.h"
#include <gloperate-qt/qt-includes-begin.h>
#include <QResizeEvent>
#include <QKeyEvent>
#include <gloperate-qt/qt-includes-end.h>
#include <globjects/globjects.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/tools/ScreenshotTool.h>


using namespace gloperate;
namespace gloperate_qt
{


/**
*  @brief
*    Constructor
*/
QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager)
: QtOpenGLWindowBase()
, m_resourceManager(resourceManager)
, m_timePropagator(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
QtOpenGLWindow::QtOpenGLWindow(gloperate::ResourceManager & resourceManager, const QSurfaceFormat & format)
: QtOpenGLWindowBase(format)
, m_resourceManager(resourceManager)
, m_timePropagator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
QtOpenGLWindow::~QtOpenGLWindow()
{
}

/**
*  @brief
*    Get used painter
*/
Painter * QtOpenGLWindow::painter() const
{
    return m_painter;
}

/**
*  @brief
*    Set used painter
*/
void QtOpenGLWindow::setPainter(Painter * painter)
{
    // Save painter
    m_painter = painter;

    // Destroy old time propagator
    m_timePropagator.reset(nullptr);

    // Check for virtual time capability
    if (painter->supports<gloperate::AbstractVirtualTimeCapability>()) {
        // Create a time propagator that updates the virtual time
        m_timePropagator.reset(new TimePropagator(this, painter->getCapability<gloperate::AbstractVirtualTimeCapability>()));
    }
}

void QtOpenGLWindow::onInitialize()
{
    // Initialize globjects
    globjects::init();
    IF_DEBUG(globjects::DebugMessage::enable(true);)

    // Initialize painter
    if (m_painter) {
        m_painter->initialize();
    }
}

void QtOpenGLWindow::onResize(QResizeEvent * event)
{
    if (m_painter) {
        // Check if the painter supports the viewport capability
        AbstractViewportCapability * viewportCapability = m_painter->getCapability<AbstractViewportCapability>();
        if (viewportCapability) {
            // Resize painter
            viewportCapability->setViewport(0, 0, event->size().width(), event->size().height());
        }
    }
}

void QtOpenGLWindow::onPaint()
{
    if (m_painter) {
        // Call painter
        m_painter->paint();
    }
}

void QtOpenGLWindow::keyPressEvent(QKeyEvent * event)
{
    makeCurrent();
    if (event->key() == Qt::Key_F10)
    {
        if (ScreenshotTool::isApplicableTo(painter()))
        {
            ScreenshotTool screenshot(painter(), m_resourceManager);

            screenshot.initialize();

            screenshot.save("screenshot.png");
        }
    }

    doneCurrent();
}


} // namespace gloperate-qt
