
#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <reflectionzeug/base/Color.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/AbstractStage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{

class AbstractViewportCapability;
class AbstractTargetFramebufferCapability;

} // namespace gloperate


namespace gloperate_text
{


class GlyphVertexCloud;
class GlyphRenderer;

class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::AbstractStage
{
public:
    GlyphRenderStage();
    virtual ~GlyphRenderStage();

    gloperate::InputSlot<GlyphVertexCloud> vertices;
    gloperate::InputSlot<reflectionzeug::Color> fontColor;
    gloperate::InputSlot<float> distanceThreshold;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractTargetFramebufferCapability *> targetFramebuffer;
protected:
    virtual void initialize() override;
    virtual void process() override;

    std::unique_ptr<GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
