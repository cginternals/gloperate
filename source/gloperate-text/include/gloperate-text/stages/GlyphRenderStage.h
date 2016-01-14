
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


class GlyphVertexCloud;

class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::AbstractStage
{
public:
    GlyphRenderStage();
    virtual ~GlyphRenderStage();

    gloperate::AbstractInputSlot<GlyphVertexCloud> vertices;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<gloperate::AbstractProjectionCapability *> projection;
    gloperate::InputSlot<gloperate::AbstractTypedRenderTargetCapability *> typedRenderTarget;

    gloperate::Data<globjects::Texture *> colorTexture;
    gloperate::Data<globjects::Texture *> idTexture;
    gloperate::Data<globjects::Texture *> normalTexture;
    gloperate::Data<globjects::Texture *> depthTexture;

protected:
    virtual void initialize() override;
    virtual void process() override;

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
};


} // namespace gloperate_text
