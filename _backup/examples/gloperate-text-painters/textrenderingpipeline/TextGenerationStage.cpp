
#include "TextGenerationStage.h"

#include <glm/gtc/random.hpp>

#include <gloperate-text/TextConverter.h>

#include "TextGenerationAlgorithm.h"


TextGenerationStage::TextGenerationStage()
: AbstractStage("TextGenerationStage")
{
    addInput("textGenerationAlgorithm", textGenerationAlgorithm);
    addInput("staticText", staticText);
    addInput("length", length);
    addInput("encoding", encoding);

    addOutput("text", text);
}

TextGenerationStage::~TextGenerationStage()
{
}

void TextGenerationStage::process()
{
    if (textGenerationAlgorithm.hasChanged())
    {
        alwaysProcess(textGenerationAlgorithm.data() == TextGenerationAlgorithm::Random);
    }

    switch (textGenerationAlgorithm.data())
    {
    case TextGenerationAlgorithm::Incrementing:
        {
            text.data().characters() = std::u32string();
            text.data().characters().resize(length.data());

            for (size_t i = 0; i < length.data(); ++i)
            {
                text.data().characters()[i] = 65+i;
            }
        }
        break;
    case TextGenerationAlgorithm::Random:
        {
            text.data().characters() = std::u32string();
            text.data().characters().resize(length.data());

            const auto maxIndex = staticText.data().size()-1;

            for (size_t i = 0; i < length.data(); ++i)
            {
                text.data().characters()[i] = staticText.data().at(static_cast<char32_t>(glm::linearRand(0.0, 1.0) * maxIndex));
            }
        }
        break;
    case TextGenerationAlgorithm::StaticText:
        text.data().characters().clear();
        gloperate_text::TextConverter::encode(staticText.data(), encoding.data(), text.data().characters());
        break;
    default:
        text.data().characters() = std::u32string({ 65, 66, 67, 68, 69, 70 });
    }

    invalidateOutputs();
}
