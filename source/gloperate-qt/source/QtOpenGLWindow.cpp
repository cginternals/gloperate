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
#include <gloperate/capabilities/AbstractTargetFramebufferCapability.h>
#include <gloperate/resources/ResourceManager.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>


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
        gloperate::Painter * p = painter();
        if (p)
        {
            AbstractViewportCapability * viewportCapability = p->getCapability<AbstractViewportCapability>();
            AbstractTargetFramebufferCapability * fboCapability = p->getCapability<AbstractTargetFramebufferCapability>();

            if (fboCapability && viewportCapability)
            {
                globjects::Framebuffer * fbo = new globjects::Framebuffer();

                globjects::Texture * color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
                color->image2D(0, gl::GL_RGBA, viewportCapability->width(), viewportCapability->height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

                globjects::Renderbuffer * depth = new globjects::Renderbuffer();
                // [TODO] Check for availability of depth format
                depth->storage(gl::GL_DEPTH_COMPONENT32, viewportCapability->width(), viewportCapability->height());

                fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, color);
                fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, depth);

                globjects::Framebuffer * oldFbo = fboCapability->framebuffer();
                fboCapability->setFramebuffer(fbo);

                p->paint();

                // [TODO] handle filename
                m_resourceManager.storeTexture("screenshot.png", color);

                fboCapability->setFramebuffer(oldFbo);
            }
        }
    }

    doneCurrent();
}


} // namespace gloperate-qt
