
#include "TextPreparationStage.h"

#include <glm/gtc/random.hpp>

#include <gloperate/painter/AbstractViewportCapability.h>

#include <gloperate-text/TextConverter.h>
#include <gloperate-text/TextLayouter.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/Text.h>

#include "TextGenerationAlgorithm.h"


TextPreparationStage::TextPreparationStage()
: AbstractStage("TextPreparationStage")
{
    addInput("font", font);
    addInput("text", text);
    addInput("viewport", viewport);
    addInput("xRepeat", xRepeat);
    addInput("yRepeat", yRepeat);
    addInput("jitterRadius", jitterRadius);

    addOutput("vertexCloud", vertexCloud);

    alwaysProcess(true);
}

TextPreparationStage::~TextPreparationStage()
{
}

void TextPreparationStage::initialize()
{
    m_layouter.reset(new gloperate_text::TextLayouter);
}

void TextPreparationStage::process()
{
    if (jitterRadius.hasChanged())
    {
        alwaysProcess(jitterRadius.data() > 0);
    }

    const auto textSize = text.data().characters().size();

    vertexCloud.data().vertices.resize(textSize * xRepeat.data() * yRepeat.data());

    vertexCloud.data().glyphTexture = font.data()->glyphTexture();

    for (size_t y = 0; y < yRepeat.data(); ++y)
    {
        for (size_t x = 0; x < xRepeat.data(); ++x)
        {
            glm::vec2 start = glm::vec2(x / float(xRepeat.data()) * 2.0 - 1.0, y / float(yRepeat.data()) * 2.0 - 1.0);
            glm::vec2 end = glm::vec2((x+1) / float(xRepeat.data()) * 2.0 - 1.0, (y+1) / float(yRepeat.data()) * 2.0 - 1.0);

            if (jitterRadius.data() > 0.0)
            {
                start += glm::vec2(glm::diskRand(float(jitterRadius.data())) / glm::vec2(viewport.data()->width(), viewport.data()->height()));
                end += glm::vec2(glm::diskRand(float(jitterRadius.data())) / glm::vec2(viewport.data()->width(), viewport.data()->height()));
            }

            m_layouter->basicLayout(text.data(), font.data(), &vertexCloud.data().vertices[(xRepeat.data() * y + x) * textSize], start, end);
        }
    }

    vertexCloud.data().verticesChanged();

    invalidateOutputs();
}
