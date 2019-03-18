
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
    texture.invalidate();
}

void TextureLoadStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_texture = nullptr;

    texture.setValue(nullptr);
}

void TextureLoadStage::onProcess()
{
    // Load texture
    auto tex = m_environment->resourceManager()->load<globjects::Texture>((*filename).path());
    m_texture = tex ? std::unique_ptr<globjects::Texture>(tex) : globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    // Update outputs
    texture.setValue(m_texture.get());
}


} // namespace gloperate
