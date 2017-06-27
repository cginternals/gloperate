
#include <gloperate/stages/base/TextureFromRenderTargetExtractionStage.h>

#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/ColorRenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureFromRenderTargetExtractionStage, gloperate::Stage)


TextureFromRenderTargetExtractionStage::TextureFromRenderTargetExtractionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TextureFromRenderTargetExtractionStage", name)
, colorRenderTarget("colorRenderTarget", this)
, texture("texture", this)
{
}

TextureFromRenderTargetExtractionStage::~TextureFromRenderTargetExtractionStage()
{
}

void TextureFromRenderTargetExtractionStage::onProcess()
{
    switch (colorRenderTarget->type())
    {
    case RenderTargetType::Texture:
        texture.setValue(colorRenderTarget->textureAttachment());
        break;
    case RenderTargetType::UserDefinedFBOAttachment:
        {
            auto attachment = colorRenderTarget->framebufferAttachment()->asTextureAttachment();

            texture.setValue(attachment ? attachment->texture() : nullptr);
        }
        break;
    default:
        texture.setValue(nullptr);
    }
}


} // namespace gloperate
