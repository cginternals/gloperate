
#pragma once


#include <vector>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace openll
{


class FontFace;
class Label;
class GlyphVertexCloud;


} // namespace openll


namespace gloperate_text
{


class GLOPERATE_TEXT_API GlyphPreparationStage : public gloperate::Stage
{
public:
    Input<openll::FontFace *> font;
    Input<std::vector<openll::Label> *> sequences;
    Input<bool> optimized;

    Output<openll::GlyphVertexCloud *> vertexCloud;


public:
    explicit GlyphPreparationStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~GlyphPreparationStage();


protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<openll::GlyphVertexCloud> m_vertexCloud;
};


} // namespace gloperate_text
