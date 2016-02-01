
#pragma once

#include <string>
#include <cstdint>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate-text/Text.h>


namespace gloperate_text
{
    enum class Encoding : unsigned int;
} // namespace gloperate_text

enum class TextGenerationAlgorithm : unsigned int;

class TextGenerationStage : public gloperate::AbstractStage
{
public:
    TextGenerationStage();
    virtual ~TextGenerationStage();

    virtual void process() override;

public:
    gloperate::InputSlot<TextGenerationAlgorithm> textGenerationAlgorithm;
    gloperate::InputSlot<std::string> staticText;
    gloperate::InputSlot<std::uint32_t> length;
    gloperate::InputSlot<gloperate_text::Encoding> encoding;

    gloperate::Data<gloperate_text::Text> text;
};
