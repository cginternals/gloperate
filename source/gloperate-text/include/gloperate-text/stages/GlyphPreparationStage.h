
#pragma once

#include <vector>

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/InputSlot.h>

#include <gloperate-text/GlyphVertexCloud.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

class FontFace;
class GlyphSequence;


class GLOPERATE_TEXT_API GlyphPreparationStage : public gloperate::AbstractStage
{
public:
    GlyphPreparationStage();
    virtual ~GlyphPreparationStage();

    virtual void process() override;

public:
    gloperate::InputSlot<FontFace *> font;

    gloperate::InputSlot<std::vector<GlyphSequence>> sequences;
    gloperate::InputSlot<bool> optimized;

    gloperate::Data<GlyphVertexCloud> vertexCloud;
};


} // namespace gloperate_text
