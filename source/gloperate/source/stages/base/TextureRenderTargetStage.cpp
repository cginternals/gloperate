
#include <gloperate/stages/base/TextureRenderTargetStage.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/RenderTarget.h>


using namespace gl;
using namespace globjects;


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureRenderTargetStage, gloperate::Stage)


TextureRenderTargetStage::TextureRenderTargetStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TextureRenderTargetStage", name)
, internalFormat("internalFormat", this)
, format("format", this)
, type("type", this)
, size("size", this)
, texture("texture", this)
, renderTarget("renderTarget", this)
{
}

TextureRenderTargetStage::~TextureRenderTargetStage()
{
}

void TextureRenderTargetStage::onContextInit(gloperate::AbstractGLContext *)
{
    // Create new texture
    m_texture = Texture::createDefault(GL_TEXTURE_2D);

    // Create wrapping render target
    m_renderTarget = cppassist::make_unique<RenderTarget>();
    m_renderTarget->setTarget(m_texture.get());
}

void TextureRenderTargetStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_texture      = nullptr;
    m_renderTarget = nullptr;
}

void TextureRenderTargetStage::onProcess()
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

    // Update outputs
    texture.setValue(m_texture.get());
    renderTarget.setValue(m_renderTarget.get());
}


} // namespace gloperate
