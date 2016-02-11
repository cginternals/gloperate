
#include "TextRenderingPipeline.h"

#include <string>

#include <gloperate/gloperate.h>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/stages/FontImporterStage.h>
#include <gloperate-text/stages/GlyphPreparationStage.h>
#include <gloperate-text/stages/GlyphRenderStage.h>


//#include "TextGenerationAlgorithm.h"
//#include "TextGenerationStage.h"
//#include "TextPreparationStage.h"
#include "GlyphSequenceDemoStage.h"

namespace
{
    const auto lorem = 
R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed rutrum gravida sapien sit amet egestas. Cras iaculis in urna non finibus. Interdum et malesuada fames ac ante ipsum primis in faucibus. Praesent consectetur velit ex, sit amet fringilla lacus vehicula nec. In mollis posuere scelerisque. Aliquam viverra lacinia scelerisque.Vivamus eu iaculis leo. Etiam fringilla libero ac metus fermentum, non congue turpis tristique. Etiam pulvinar eu mi a rhoncus. Donec purus nibh, egestas fringilla lorem non, luctus facilisis elit. Phasellus non consequat est, sit amet vestibulum felis.

Proin commodo nisi at risus tempor aliquam. Morbi vehicula condimentum mauris, eget viverra lorem sagittis eget. Vivamus ante augue, volutpat ac ex posuere, dignissim tristique sapien. Praesent feugiat vitae justo vel auctor. Nam pharetra neque eget ullamcorper convallis. Etiam quam nisl, eleifend at risus ut, interdum vestibulum est. Mauris venenatis rhoncus sagittis. Nam convallis, dui vitae varius facilisis, leo mauris maximus metus, et fringilla sem nibh ut arcu.

Vivamus at tortor sed tortor gravida tincidunt id et tellus. Phasellus sagittis, mi at eleifend consequat, purus mauris ultrices mi, non mollis nibh tortor quis diam. Quisque ullamcorper posuere dui, nec facilisis orci egestas eu. Sed vel libero eget neque gravida tristique. Quisque eu semper nisl. Aenean faucibus magna purus, at congue sem imperdiet sit amet. Cras commodo velit eu elit scelerisque tristique.)";
}

TextRenderingPipeline::TextRenderingPipeline()
: resourceManager(nullptr)
, fontFilename(gloperate::dataPath() + "/gloperate-text/fonts/segoeuil.txt")
, string(lorem)
//, time(nullptr)
, pixelPerInch(72.f)
, fontSize(16.f)
, origin(-1.f, 1.f)
, margins(64.f, 32.f, 64.f, 32.f) // t r b l
, wordWrap(true)
, lineWidth(756.f)
, alignment(gloperate_text::Alignment::LeftAligned)
, targetFBO(nullptr)
, viewport(nullptr)
{
    auto fontImport = new gloperate_text::FontImporterStage;
    auto demo = new GlyphSequenceDemoStage;
    auto glyphPreparation = new gloperate_text::GlyphPreparationStage;
    auto glyphRendering = new gloperate_text::GlyphRenderStage;

    fontImport->resourceManager = resourceManager;
    fontImport->fontFilePath = fontFilename;

    demo->string = string;
    demo->font = fontImport->font;
    demo->fontSize = fontSize;
    demo->pixelPerInch = pixelPerInch;
    demo->viewport = viewport;
    demo->origin = origin;
    demo->margins = margins;
    demo->wordWrap = wordWrap;
    demo->lineWidth = lineWidth;
    demo->alignment = alignment;

    glyphPreparation->font = fontImport->font;
    glyphPreparation->sequences = demo->sequences;

    glyphRendering->pixelPerInch = pixelPerInch;
    glyphRendering->vertexCloud = glyphPreparation->vertexCloud;
    glyphRendering->viewport = viewport;
    glyphRendering->targetFramebuffer = targetFBO;

    addStages(
        fontImport
    ,   demo
    ,   glyphPreparation
    ,   glyphRendering);
}

TextRenderingPipeline::~TextRenderingPipeline()
{
}
