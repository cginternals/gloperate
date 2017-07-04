
#include <gloperate/stages/base/TextureFromRenderTargetExtractionStage.h>

#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>
#include <gloperate/rendering/DepthStencilRenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TextureFromRenderTargetExtractionStage, gloperate::Stage)


TextureFromRenderTargetExtractionStage::TextureFromRenderTargetExtractionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TextureFromRenderTargetExtractionStage", name)
, colorRenderTarget("colorRenderTarget", this)
, depthRenderTarget("depthRenderTarget", this)
, stencilRenderTarget("stencilRenderTarget", this)
, depthStencilRenderTarget("depthStencilRenderTarget", this)
, texture("texture", this)
{
}

TextureFromRenderTargetExtractionStage::~TextureFromRenderTargetExtractionStage()
{
}

void TextureFromRenderTargetExtractionStage::onProcess()
{
    if (*colorRenderTarget)
    {
        return extractTexture(*colorRenderTarget);
    }

    if (*depthRenderTarget)
    {
        return extractTexture(*depthRenderTarget);
    }

    if (*stencilRenderTarget)
    {
        return extractTexture(*stencilRenderTarget);
    }

    if (*depthStencilRenderTarget)
    {
        return extractTexture(*depthStencilRenderTarget);
    }

    texture.setValue(nullptr);
}

void TextureFromRenderTargetExtractionStage::extractTexture(AbstractRenderTarget * renderTarget)
{
    switch (renderTarget->currentTargetType())
    {
    case RenderTargetType::Texture:
        texture.setValue(renderTarget->textureAttachment());
        break;
    case RenderTargetType::UserDefinedFBOAttachment:
        {
            auto attachment = renderTarget->framebufferAttachment()->asTextureAttachment();

            texture.setValue(attachment ? attachment->texture() : nullptr);
        }
        break;
    default:
        texture.setValue(nullptr);
    }
}


} // namespace gloperate
