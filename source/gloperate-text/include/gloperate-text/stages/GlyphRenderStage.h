
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

class GlyphRenderer;
class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::AbstractStage
{
public:
    GlyphRenderStage();
    virtual ~GlyphRenderStage();

    gloperate::InputSlot<GlyphVertexCloud> vertexCloud;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractTargetFramebufferCapability *> targetFramebuffer;

    gloperate::InputSlot<int> quality;

protected:
    virtual void initialize() override;
    virtual void process() override;

protected:
    std::unique_ptr<GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
