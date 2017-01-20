
#include <gloperate/stages/base/TextureStage.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/RenderTarget.h>


using namespace gl;
using namespace globjects;


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureStage, gloperate::Stage)


TextureStage::TextureStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TextureStage", name)
, internalFormat("internalFormat", this)
, format("format", this)
, type("type", this)
, size("size", this)
, texture("texture", this)
, renderTarget("renderTarget", this)
{
}

TextureStage::~TextureStage()
{
}

void TextureStage::onContextInit(gloperate::AbstractGLContext *)
{
    // Create new texture
    m_texture = Texture::createDefault(GL_TEXTURE_2D);
    // Create wrapping render target
    m_renderTarget = new RenderTarget();
    m_renderTarget->setTarget(m_texture);
}

void TextureStage::onContextDeinit(AbstractGLContext *)
{
    // Release texture
    m_texture = nullptr;
    // Release render target
    m_renderTarget = nullptr;
}

void TextureStage::onProcess(gloperate::AbstractGLContext *)
{
    // Check if texture has been created successfully
    if (!m_texture.get())
    {
        return;
    }

    // Create texture image
    const auto width  = (*size)[2];
    const auto height = (*size)[3];
    m_texture->image2D(0, *internalFormat, width, height, 0, *format, *type, nullptr);

    // Update output values
    texture.setValue(m_texture);
    renderTarget.setValue(m_renderTarget);
}


} // namespace gloperate
