
#pragma once

#include <vector>

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>

#include <gloperate-text/GlyphVertexCloud.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

class FontFace;
class GlyphSequence;


class GLOPERATE_TEXT_API GlyphPreparationStage : public gloperate::Stage
{
public:
    GlyphPreparationStage(gloperate::Environment * environment, const std::string & name = "GlyphPreparationStage");
    virtual ~GlyphPreparationStage();

    virtual void onProcess(gloperate::AbstractGLContext * context) override;

public:
    Input<FontFace *> font;

    Input<std::vector<GlyphSequence>> sequences;
    Input<bool> optimized;

    Output<GlyphVertexCloud> vertexCloud;
};


} // namespace gloperate_text
