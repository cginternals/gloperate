
#pragma once

#include <string>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate-text/geometry/GlyphVertexCloud.h>


namespace gloperate
{
    class AbstractViewportCapability;
}

namespace gloperate_text
{
    class FontFace;
    class TextLayouter;
    class Text;
}


class TextPreparationStage : public gloperate::AbstractStage
{
public:
    TextPreparationStage();
    virtual ~TextPreparationStage();

    virtual void initialize() override;
    virtual void process() override;

public:
    gloperate::InputSlot<gloperate_text::FontFace *> font;
    gloperate::InputSlot<gloperate_text::Text> text;
    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<std::uint32_t> xRepeat;
    gloperate::InputSlot<std::uint32_t> yRepeat;
    gloperate::InputSlot<std::uint32_t> jitterRadius;

    gloperate::Data<gloperate_text::GlyphVertexCloud> vertexCloud;

protected:
    std::unique_ptr<gloperate_text::TextLayouter> m_layouter;
};
