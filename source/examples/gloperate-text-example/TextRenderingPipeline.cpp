
#include "TextRenderingPipeline.h"

#include <gloperate/gloperate.h>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/LineAnchor.h>
#include <gloperate-text/stages/FontImporterStage.h>
#include <gloperate-text/stages/GlyphPreparationStage.h>
#include <gloperate-text/stages/GlyphRenderStage.h>

#include "GlyphSequenceDemoStage.h"


namespace
{


const auto lorem = 
R"(Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.)";


} // unnamed namespace


CPPEXPOSE_COMPONENT(TextRenderingPipeline, gloperate::Stage)


TextRenderingPipeline::TextRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: gloperate::Pipeline{ environment, "TextRenderingPipeline", name }
, renderInterface{ this }
, fontFilename{ "fontFilename", this, gloperate::dataPath() + "/gloperate-text/fonts/opensansr36.fnt" }
, string      { "string",       this, lorem }
, numChars    { "numChars",     this, 0 }
, fontSize    { "fontSize",     this, 1.0f }
, origin      { "origin",       this, { 0.0f, 0.5f } }
, margins     { "margins",      this, { 0.0f, 0.0f, 0.0f, 0.0f } } // t r b l
, wordWrap    { "wordWrap",     this, true }
, lineWidth   { "lineWidth",    this, 400.0f }
, alignment   { "alignment",    this, gloperate_text::Alignment::Centered }
, lineAnchor  { "lineAnchor",   this, gloperate_text::LineAnchor::Baseline }
, optimized   { "optimized",    this, true }
{
    auto fontImport = new gloperate_text::FontImporterStage{ environment, "FontImport" };
    fontImport->fontFilePath << fontFilename;
    addStage(fontImport);

    auto demo = new GlyphSequenceDemoStage{ environment, "DemoSequence" };
    demo->string       << string;
    demo->numChars     << numChars;
    demo->font         << fontImport->font;
    demo->fontSize     << fontSize;
    demo->viewport     << renderInterface.deviceViewport;
    demo->origin       << origin;
    demo->margins      << margins;
    demo->wordWrap     << wordWrap;
    demo->lineWidth    << lineWidth;
    demo->alignment    << alignment;
    demo->lineAnchor   << lineAnchor;
    demo->pixelPerInch.setValue(72.0f);
    addStage(demo);

    auto glyphPreparation = new gloperate_text::GlyphPreparationStage{ environment, "GlyphPreparation" };
    glyphPreparation->font      << fontImport->font;
    glyphPreparation->sequences << demo->sequences;
    glyphPreparation->optimized << optimized;
    addStage(glyphPreparation);

    auto glyphRendering = new gloperate_text::GlyphRenderStage{ environment, "GlyphRendering" };
    glyphRendering->vertexCloud       << glyphPreparation->vertexCloud;
    glyphRendering->viewport          << renderInterface.deviceViewport;
    glyphRendering->targetFramebuffer << renderInterface.targetFBO;
    addStage(glyphRendering);

    renderInterface.rendered << glyphRendering->rendered;
}
