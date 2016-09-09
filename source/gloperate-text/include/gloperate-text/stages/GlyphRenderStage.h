
#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/base/Color.h>

#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>
#include <gloperate-text/GlyphVertexCloud.h>

namespace gloperate
{


} // namespace gloperate


namespace gloperate_text
{

class GlyphRenderer;
class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::Stage
{
public:
    GlyphRenderStage(gloperate::Environment * environment, const std::string & name = "GlyphRenderStage");
    virtual ~GlyphRenderStage();

    Input<GlyphVertexCloud> vertexCloud;

    Input<glm::vec4> viewport;
    Input<globjects::Framebuffer *> targetFramebuffer;

protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

protected:
    std::unique_ptr<GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
