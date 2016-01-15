
#include "TextPreparationStage.h"

#include <glm/gtc/random.hpp>

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
    addInput("time", time);
    addInput("xRepeat", xRepeat);
    addInput("yRepeat", yRepeat);
    addInput("jitterRadius", jitterRadius);

    addOutput("vertexCloud", vertexCloud);
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
    vertexCloud.data().vertices.resize(text.data().characters().size() * xRepeat.data() * yRepeat.data());

    vertexCloud.data().glyphTexture = font.data()->glyphTexture();

    for (size_t y = 0; y < yRepeat.data(); ++y)
    {
        for (size_t x = 0; x < xRepeat.data(); ++x)
        {
            glm::vec3 start = glm::vec3(x / float(xRepeat.data()) * 2.0 - 1.0, y / float(yRepeat.data()) * 2.0 - 1.0, 0);
            glm::vec3 end = glm::vec3((x+1) / float(xRepeat.data()) * 2.0 - 1.0, (y+1) / float(yRepeat.data()) * 2.0 - 1.0, 0);

            m_layouter->basicLayout(text.data(), font.data(), &vertexCloud.data().vertices[0], start, end);
        }
    }

    vertexCloud.data().verticesChanged();

    invalidateOutputs();
}
