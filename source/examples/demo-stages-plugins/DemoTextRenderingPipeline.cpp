
#include "DemoTextRenderingPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/ClearStage.h>

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


CPPEXPOSE_COMPONENT(DemoTextRenderingPipeline, gloperate::Stage)


DemoTextRenderingPipeline::DemoTextRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: gloperate::Pipeline{ environment, "DemoTextRenderingPipeline", name }
, canvasInterface{ this }
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
    auto fontImport = cppassist::make_unique<gloperate_text::FontImporterStage>(environment, "FontImport");
    fontImport->fontFilePath << fontFilename;

    auto demo = cppassist::make_unique<GlyphSequenceDemoStage>(environment, "DemoSequence");
    demo->string       << string;
    demo->numChars     << numChars;
    demo->font         << fontImport->font;
    demo->fontSize     << fontSize;
    demo->viewport     << canvasInterface.viewport;
    demo->origin       << origin;
    demo->margins      << margins;
    demo->wordWrap     << wordWrap;
    demo->lineWidth    << lineWidth;
    demo->alignment    << alignment;
    demo->lineAnchor   << lineAnchor;
    demo->pixelPerInch.setValue(72.0f);

    auto glyphPreparation = cppassist::make_unique<gloperate_text::GlyphPreparationStage>(environment, "GlyphPreparation");
    glyphPreparation->font      << fontImport->font;
    glyphPreparation->sequences << demo->sequences;
    glyphPreparation->optimized << optimized;

    auto clearStage = cppassist::make_unique<gloperate::ClearStage>(environment, "ClearStage");
    clearStage->renderInterface.viewport << canvasInterface.viewport;
    clearStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    clearStage->createInput("ColorValue") << canvasInterface.backgroundColor;
    auto clearedColorTarget = clearStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");

    auto glyphRendering = cppassist::make_unique<gloperate_text::GlyphRenderStage>(environment, "GlyphRendering");
    glyphRendering->vertexCloud << glyphPreparation->vertexCloud;
    glyphRendering->renderInterface.viewport << canvasInterface.viewport;
    glyphRendering->createInput("Color") << *clearedColorTarget;
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *glyphRendering->createOutput<gloperate::ColorRenderTarget *>("ColorOut");

    addStage(std::move(fontImport));
    addStage(std::move(demo));
    addStage(std::move(glyphPreparation));
    addStage(std::move(clearStage));
    addStage(std::move(glyphRendering));
}
