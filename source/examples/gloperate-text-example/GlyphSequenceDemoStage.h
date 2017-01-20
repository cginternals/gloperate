
#pragma once


#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/GlyphSequence.h>


namespace gloperate_text
{
enum class LineAnchor : unsigned char;
enum class Alignment : unsigned char;
class FontFace;
}


class GlyphSequenceDemoStage : public gloperate::Stage
{
public:
    Input<std::string> string;
    Input<uint32_t> numChars;

    Input<gloperate_text::FontFace *> font;
    Input<float> fontSize;
    Input<bool> wordWrap;
    Input<float> lineWidth;
    Input<gloperate_text::Alignment> alignment;
    Input<gloperate_text::LineAnchor> lineAnchor;

    Input<glm::vec2> origin;
    Input<glm::vec4> margins;
    
    Input<float> pixelPerInch;
    Input<glm::vec4> viewport;

    Output<std::vector<gloperate_text::GlyphSequence> *> sequences;


public:
    explicit GlyphSequenceDemoStage(gloperate::Environment * environment, const std::string & name = "");


protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;


protected:
    std::vector<gloperate_text::GlyphSequence> m_sequences;
};
