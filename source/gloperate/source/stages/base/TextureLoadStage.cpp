
#include <gloperate/stages/base/TextureLoadStage.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


TextureLoadStage::TextureLoadStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
, filename(this, "filename")
, texture (this, "texture")
, m_reloadTexture(false)
{
}

TextureLoadStage::~TextureLoadStage()
{
}

void TextureLoadStage::onContextInit(AbstractGLContext *)
{
}

void TextureLoadStage::onContextDeinit(AbstractGLContext *)
{
}

void TextureLoadStage::onProcess(AbstractGLContext *)
{
    if (!m_texture.get() || m_reloadTexture)
    {
        loadTexture();
    }

    texture.setValue(m_texture.get());
}

void TextureLoadStage::loadTexture()
{
    // Load texture from file
    m_texture = m_viewerContext->resourceManager()->load<globjects::Texture>(
        *filename
    );
}


} // namespace gloperate
