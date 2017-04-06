
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <iostream>


namespace gloperate_qtquick
{


RenderItemRenderer::RenderItemRenderer()
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
    std::cout << "render()" << std::endl;

    QOpenGLFunctions * gl = QOpenGLContext::currentContext()->functions();
    gl->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderItemRenderer::synchronize(QQuickFramebufferObject *)
{
    std::cout << "synchronize()" << std::endl;
}


} // namespace gloperate_qtquick
