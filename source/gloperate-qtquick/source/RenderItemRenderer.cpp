
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>

#include <glm/vec4.hpp>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Canvas.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/QtOpenGL.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>


using namespace cppassist;


namespace gloperate_qtquick
{


// NOTE: All function in RenderItem are called from the render thread

void RenderItemRenderer::synchronize(QQuickFramebufferObject *)
{

    // [TODO]
}

QOpenGLFramebufferObject * RenderItemRenderer::createFramebufferObject(const QSize & size)
{
    // Get QML window
    auto * window = m_renderItem->window();

    // Initialize context before rendering the first time
    if (!m_contextInitialized)
    {
        // Initialize glbinding and globjects in context
        Utils::initContext([window] (const char * name) -> glbinding::ProcAddress {
            return gloperate_qt::QtOpenGL::getProcAddress(window->openglContext(), name);
        });

        // Print context info
        info() << std::endl
            << "OpenGL Version:  " << gloperate::GLContextUtils::version() << std::endl
            << "OpenGL Vendor:   " << gloperate::GLContextUtils::vendor() << std::endl
            << "OpenGL Renderer: " << gloperate::GLContextUtils::renderer() << std::endl;

        // Create context wrapper
        m_context = cppassist::make_unique<gloperate_qt::GLContext>(
            window,
            window->openglContext(),
            false);

        // Make sure that context is still active
        window->openglContext()->makeCurrent(window);

        // Finish FBO setup
        initializeFboAttachments();

        m_contextInitialized = true;
    }
    else
    {
        // Activate context
        Utils::setGlobjectsContext();
        window->openglContext()->makeCurrent(window);
    }

    // Create new FBO
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);
    auto * fbo = new QOpenGLFramebufferObject(size, format);

    // Create globjects FBO wrapper
    configureFbo(fbo->handle(), size.width(), size.height());

    // Initialize canvas before rendering the first time
    if (!m_canvasInitialized)
    {
        m_canvas->setOpenGLContext(m_context.get());

        m_canvasInitialized = true;
    }

    // Set viewport size
    auto ratio = window->devicePixelRatio();
    m_canvas->setViewport(
        glm::vec4(0, 0, m_renderItem->width() * ratio, m_renderItem->height() * ratio)
    );

    // This function is called by Qt. We must not reset the context here because
    // Qt still assumes it to be active.

    // window->openglContext()->doneCurrent();

    return fbo;
}

void RenderItemRenderer::render()
{
    // Render canvas
    m_canvas->render(m_innerFbo.get());

    // Render screen aligned quad
    renderTexture();

    // Reset OpenGL state for QML
    m_renderItem->window()->resetOpenGLState();
}


} // namespace gloperate_qtquick
