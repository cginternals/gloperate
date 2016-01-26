
#include "TextRenderingPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/resources/ResourceManager.h>

#include <gloperate-text/FontFace.h>
#include <gloperate-text/stages/FontImporterStage.h>
#include <gloperate-text/stages/GlyphRenderStage.h>
#include <gloperate-text/Encoding.h>

#include "TextGenerationAlgorithm.h"
#include "TextGenerationStage.h"
#include "TextPreparationStage.h"


TextRenderingPipeline::TextRenderingPipeline()
: resourceManager(nullptr)
, fontFilename(gloperate::dataPath() + "/gloperate-text/fonts/test.txt")
, textGenerationAlgorithm(TextGenerationAlgorithm::StaticText)
, staticText("Test!")
, length(5)
, encoding(gloperate_text::Encoding::ASCII)
, time(nullptr)
, xRepeat(1)
, yRepeat(1)
, jiggleRadius(0)
, targetFBO(nullptr)
, viewport(nullptr)
, fontColor(reflectionzeug::Color(255, 255, 255))
, distanceThreshold(0.5f)
{
    auto fontImport = new gloperate_text::FontImporterStage;
    auto textGeneration = new TextGenerationStage;
    auto textPreparation = new TextPreparationStage;
    auto textRendering = new gloperate_text::GlyphRenderStage;

    fontImport->resourceManager = resourceManager;
    fontImport->fontFilePath = fontFilename;

    textGeneration->textGenerationAlgorithm = textGenerationAlgorithm;
    textGeneration->staticText = staticText;
    textGeneration->length = length;
    textGeneration->encoding = encoding;

    textPreparation->font = fontImport->font;
    textPreparation->text = textGeneration->text;
    textPreparation->viewport = viewport;
    textPreparation->xRepeat = xRepeat;
    textPreparation->yRepeat = yRepeat;
    textPreparation->jitterRadius = jiggleRadius;

    textRendering->vertices = textPreparation->vertexCloud;
    textRendering->viewport = viewport;
    textRendering->targetFramebuffer = targetFBO;
    textRendering->fontColor = fontColor;
    textRendering->distanceThreshold = distanceThreshold;

    addStages(
        fontImport,
        textGeneration,
        textPreparation,
        textRendering
    );
}

TextRenderingPipeline::~TextRenderingPipeline()
{
}
