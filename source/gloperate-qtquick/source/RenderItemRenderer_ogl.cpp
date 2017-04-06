
#include <gloperate-qtquick/RenderItemRenderer.h>

#include <globjects/Framebuffer.h>


namespace gloperate_qtquick
{


void RenderItemRenderer::createFboWrapper(int fboId)
{
    // Reset FBO wrapper
    destroyFboWrapper();

    // Create new FBO wrapper
    m_fbo = globjects::Framebuffer::fromId(fboId).release();
}

void RenderItemRenderer::destroyFboWrapper()
{
    // Destroy FBO
    if (m_fbo)
    {
        delete m_fbo;
        m_fbo = nullptr;
    }
}


} // namespace gloperate_qtquick
