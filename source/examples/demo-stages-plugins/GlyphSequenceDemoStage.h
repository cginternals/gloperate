
#pragma once


#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/pipeline/Stage.h>

#include <openll/Label.h>


namespace openll
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

    Input<openll::FontFace *> font;
    Input<float> fontSize;
    Input<bool> wordWrap;
    Input<float> lineWidth;
    Input<openll::Alignment> alignment;
    Input<openll::LineAnchor> lineAnchor;

    Input<glm::vec2> origin;
    Input<glm::vec4> margins;
    
    Input<float> pixelPerInch;
    Input<glm::vec4> viewport;

    Output<std::vector<openll::Label> *> sequences;


public:
    explicit GlyphSequenceDemoStage(gloperate::Environment * environment, const std::string & name = "");


protected:
    virtual void onProcess() override;


protected:
    std::vector<openll::Label> m_sequences;
};
