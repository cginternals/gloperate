
#include <gloperate/stages/base/TextureLoadStage.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureLoadStage, gloperate::Stage)


TextureLoadStage::TextureLoadStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, filename("filename", this)
, texture ("texture", this)
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
    // Check if texture needs to be rebuilt
    if (!texture.isValid())
    {
        // Load texture
        loadTexture();

        // Update outputs
        texture.setValue(m_texture.get());
    }
}

void TextureLoadStage::loadTexture()
{
    // Load texture from file
    m_texture = m_environment->resourceManager()->load<globjects::Texture>(
        *filename
    );
}


} // namespace gloperate
