
#include "TextRenderingPipeline.h"

#include <string>

#include <gloperate/gloperate.h>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/LineAnchor.h>
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
R"(Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.)";
}

TextRenderingPipeline::TextRenderingPipeline()
: resourceManager(nullptr)
, fontFilename(gloperate::dataPath() + "/gloperate-text/fonts/opensansr36.fnt")
, string(lorem)
, numChars(0)
//, time(nullptr)
, pixelPerInch(72.f)
, fontSize(16.f)
, origin(0.f, 0.5f)
, margins(0.f, 0.f, 0.f, 0.f) // t r b l
, wordWrap(true)
, lineWidth(400.f)
, alignment(gloperate_text::Alignment::Centered)
, lineAnchor(gloperate_text::LineAnchor::Baseline)
, optimized(true)
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
    demo->numChars = numChars;
    demo->font = fontImport->font;
    demo->fontSize = fontSize;
    demo->pixelPerInch = pixelPerInch;
    demo->viewport = viewport;
    demo->origin = origin;
    demo->margins = margins;
    demo->wordWrap = wordWrap;
    demo->lineWidth = lineWidth;
    demo->alignment = alignment;
    demo->lineAnchor = lineAnchor;

    glyphPreparation->font = fontImport->font;
    glyphPreparation->sequences = demo->sequences;
    glyphPreparation->optimized = optimized;

    //glyphRendering->pixelPerInch = pixelPerInch;
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
