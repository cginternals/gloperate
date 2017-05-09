
#include <gloperate/stages/base/TextureLoadStage.h>

#include <glbinding/gl/enum.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureLoadStage, gloperate::Stage)


TextureLoadStage::TextureLoadStage(Environment * environment, const std::string & name)
: Stage(environment, "TextureLoadStage", name)
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
    m_texture = nullptr;
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
    auto tex = m_environment->resourceManager()->load<globjects::Texture>((*filename).path());
    m_texture = tex ? std::unique_ptr<globjects::Texture>(tex) : globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
}


} // namespace gloperate
