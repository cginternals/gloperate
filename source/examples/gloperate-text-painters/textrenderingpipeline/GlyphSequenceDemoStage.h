
#pragma once

#include <string>
#include <vector>

#include <glm/fwd.hpp>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/InputSlot.h>


namespace gloperate
{

    class AbstractViewportCapability;

} // namespace gloperate


namespace gloperate_text
{
    enum class HorizontalAlignment : unsigned char;
    enum class VerticalAlignment : unsigned char;

    class FontFace;
    class GlyphSequence;

} // namespace gloperate_text


class GlyphSequenceDemoStage : public gloperate::AbstractStage
{
public:
    GlyphSequenceDemoStage();
    virtual ~GlyphSequenceDemoStage();

    virtual void process() override;

public:
    gloperate::InputSlot<std::string> string;

    gloperate::InputSlot<gloperate_text::FontFace *> font;

    gloperate::InputSlot<float> fontSize;
    gloperate::InputSlot<float> pixelPerInch;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<glm::vec2> origin;
    gloperate::InputSlot<glm::vec4> margins;
    gloperate::InputSlot<bool> wordWrap;
    gloperate::InputSlot<float> lineWidth;
    gloperate::InputSlot<gloperate_text::HorizontalAlignment> alignment;

    gloperate::Data<std::vector<gloperate_text::GlyphSequence>> sequences;
};
