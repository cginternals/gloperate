
#pragma once

#include <vector>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class FontFace;
class GlyphSequence;
class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphPreparationStage : public gloperate::Stage
{
public:
    Input<FontFace *> font;

    Input<std::vector<GlyphSequence> *> sequences;
    Input<bool> optimized;

    Output<GlyphVertexCloud *> vertexCloud;


public:
    explicit GlyphPreparationStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~GlyphPreparationStage();


protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;


protected:
    std::unique_ptr<GlyphVertexCloud> m_vertexCloud;
};


} // namespace gloperate_text
