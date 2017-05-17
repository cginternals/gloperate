
#include <gloperate-qtquick/TextureItemRenderer.h>

#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qtquick/TextureItem.h>
#include <gloperate-qtquick/Utils.h>


namespace gloperate_qtquick
{


void TextureItemRenderer::synchronize(QQuickFramebufferObject * framebufferObject)
{
    // Get render item
    TextureItem * textureItem = static_cast<TextureItem *>(framebufferObject);

    // Get path to texture slot
    if (m_path != textureItem->path())
    {
        // Update path
        m_path = textureItem->path();
    }

    // Redraw
    // [TODO] We should only redraw if the texture has changed
    m_textureItem->update();
}

QOpenGLFramebufferObject * TextureItemRenderer::createFramebufferObject(const QSize & size)
{
    // Get QML window
    auto * window = m_textureItem->window();

    // Activate context
    Utils::setGlobjectsContext();
    window->openglContext()->makeCurrent(window);

    // Create new FBO
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(0);
    auto * fbo = new QOpenGLFramebufferObject(size, format);

    // Create globjects FBO wrapper
    configureFbo(fbo->handle(), size.width(), size.height());
    return fbo;
}

void TextureItemRenderer::render()
{
    // Render texture on a screen aligned quad
    renderTexture();

    // Reset OpenGL state for QML
    m_textureItem->window()->resetOpenGLState();
}


} // namespace gloperate_qtquick
