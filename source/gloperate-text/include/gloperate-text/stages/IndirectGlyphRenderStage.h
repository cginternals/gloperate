
#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/AbstractStage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{

class AbstractViewportCapability;
class AbstractCameraCapability;
class AbstractProjectionCapability;
class AbstractTypedRenderTargetCapability;

} // namespace gloperate


namespace gloperate_text
{


class IndirectGlyphVertexCloud;

class GLOPERATE_TEXT_API IndirectGlyphRenderStage : public gloperate::AbstractStage
{
public:
    IndirectGlyphRenderStage();
    virtual ~IndirectGlyphRenderStage();

    gloperate::AbstractInputSlot<IndirectGlyphVertexCloud> vertices;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<gloperate::AbstractProjectionCapability *> projection;
    gloperate::InputSlot<gloperate::AbstractTypedRenderTargetCapability *> typedRenderTarget;

    gloperate::AbstractData<globjects::Texture *> colorTexture;
    gloperate::AbstractData<globjects::Texture *> idTexture;
    gloperate::AbstractData<globjects::Texture *> normalTexture;
    gloperate::AbstractData<globjects::Texture *> depthTexture;

protected:
    virtual void initialize() override;
    virtual void process() override;

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
};


} // namespace gloperate_text
