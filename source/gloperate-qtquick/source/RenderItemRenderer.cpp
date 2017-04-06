
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/GLContext.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>

#include <iostream>


namespace gloperate_qtquick
{


RenderItemRenderer::RenderItemRenderer(RenderItem * renderItem)
: m_renderItem(renderItem)
, m_first(true)
{
}

RenderItemRenderer::~RenderItemRenderer()
{
}

QOpenGLFramebufferObject * RenderItemRenderer::createFramebufferObject(const QSize & size)
{
    std::cout << "createFramebufferObject(" << size.width() << ", " << size.height() << ")" << std::endl;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);
    return new QOpenGLFramebufferObject(size, format);
}

void RenderItemRenderer::render()
{
    if (m_first)
    {
        m_first = false;

m_renderItem->window()->openglContext()->makeCurrent(m_renderItem->window());

        // Initialize glbinding and globjects in context
        Utils::initContext();

        // Print context info
        std::cout << std::endl
            << "OpenGL Version:  " << gloperate::GLContextUtils::version() << std::endl
            << "OpenGL Vendor:   " << gloperate::GLContextUtils::vendor() << std::endl
            << "OpenGL Renderer: " << gloperate::GLContextUtils::renderer() << std::endl;

        auto * a = m_renderItem->window();
        auto * b = m_renderItem->window()->openglContext();

        m_renderItem->m_context = cppassist::make_unique<gloperate_qt::GLContext>(
            m_renderItem->window(),
            m_renderItem->window()->openglContext(),
            false);

m_renderItem->window()->openglContext()->makeCurrent(m_renderItem->window());
    }

    std::cout << "render()" << std::endl;

    /*
    QOpenGLFunctions * gl = QOpenGLContext::currentContext()->functions();
    gl->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */

    m_renderItem->doRender(framebufferObject()->handle());
}

void RenderItemRenderer::synchronize(QQuickFramebufferObject *)
{
    std::cout << "synchronize()" << std::endl;
}


} // namespace gloperate_qtquick
