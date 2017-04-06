
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>

#include <glm/vec4.hpp>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>

#include <iostream>


namespace gloperate_qtquick
{


RenderItemRenderer::RenderItemRenderer(RenderItem * renderItem)
: m_renderItem(renderItem)
, m_fbo(nullptr)
, m_contextInitialized(false)
, m_canvasInitialized(false)
{
}

RenderItemRenderer::~RenderItemRenderer()
{
    destroyFboWrapper();
}

void RenderItemRenderer::synchronize(QQuickFramebufferObject * framebufferObject)
{
    // Get render item
    RenderItem * renderItem = static_cast<RenderItem *>(framebufferObject);

    // Check if canvas has changed
    if (m_canvas.get() != renderItem->canvas().get())
    {
        // Copy pointer to the new canvas
        m_canvas = renderItem->canvas();

        // Initialize canvas on next draw call
        m_canvasInitialized = false;
    }
}

QOpenGLFramebufferObject * RenderItemRenderer::createFramebufferObject(const QSize & size)
{
    // Get QML window
    auto * window = m_renderItem->window();

    // Initialize context before rendering the first time
    if (!m_contextInitialized)
    {
        m_contextInitialized = true;

        // Initialize glbinding and globjects in context
        Utils::initContext();

        // Print context info
        std::cout << std::endl
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
    }

    // Create new FBO
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);
    auto * fbo = new QOpenGLFramebufferObject(size, format);

    // Create globjects FBO wrapper
    createFboWrapper(fbo->handle());

    // Set viewport size
    if (m_canvas)
    {
        auto ratio = window->devicePixelRatio();
        m_canvas->onViewport(
            glm::vec4(0, 0, m_renderItem->width() * ratio, m_renderItem->height() * ratio)
          , glm::vec4(0, 0, m_renderItem->width(),         m_renderItem->height())
        );
    }

    // Return FBO
    return fbo;
}

void RenderItemRenderer::render()
{
    // Check canvas
    if (!m_canvas)
    {
        return;
    }

    // Initialize canvas before rendering the first time
    if (!m_canvasInitialized)
    {
        m_canvas->setOpenGLContext(m_context.get());

        m_canvasInitialized = true;
    }

    // Render canvas
    m_canvas->render(m_fbo);

    // Reset OpenGL state for QML
    m_renderItem->window()->resetOpenGLState();
}


} // namespace gloperate_qtquick
