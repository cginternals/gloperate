
#pragma once

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace globjects
{
class Framebuffer;
} // namespace globjects


namespace gloperate_text
{

class GlyphRenderer;
class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::Stage
{
public:
    Input<GlyphVertexCloud *> vertexCloud;

    Input<glm::vec4> viewport;
    Input<globjects::Framebuffer *> targetFramebuffer;

    Output<bool> rendered;


public:
    explicit GlyphRenderStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~GlyphRenderStage();


protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;


protected:
    std::unique_ptr<GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
