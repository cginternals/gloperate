
#include "GlyphSequenceDemoStage.h"

#include <cppassist/string/conversion.h>

#include <gloperate-text/GlyphSequence.h>
#include <gloperate-text/FontFace.h>


GlyphSequenceDemoStage::GlyphSequenceDemoStage(gloperate::Environment * environment, const std::string & name)
: gloperate::Stage{ environment, "GlyphSequenceDemoStage", name }
, string      { "string",       this, "" }
, numChars    { "numChars",     this, 0 }
, font        { "font",         this, nullptr }
, fontSize    { "fontSize",     this, 1.0f }
, wordWrap    { "wordWrap",     this, true }
, lineWidth   { "lineWidth",    this, 400.0f }
, alignment   { "alignment",    this, gloperate_text::Alignment::Centered }
, lineAnchor  { "lineAnchor",   this, gloperate_text::LineAnchor::Baseline }
, origin      { "origin",       this, { 0.0f, 0.5f } }
, margins     { "margins",      this, { 0.0f, 0.0f, 0.0f, 0.0f } } // t r b l
, pixelPerInch{ "pixelPerInch", this, 72.0f }
, viewport    { "viewport",     this, { 0.0f, 0.0f, 0.0f, 0.0f } }
, sequences   { "sequences",    this }
{
}


void GlyphSequenceDemoStage::onProcess(gloperate::AbstractGLContext * context)
{
    m_sequences.resize(1);

    // TODO: property editor has a fixed range [0, 2 * PI], remove scaling once this is fixed!
    const auto scaledFontSize = fontSize.value() * 16.0f;
    const auto scaledLineWidth = lineWidth.value() * 160.0f;

    if (numChars.value() == 0)
    {
        m_sequences.front().setString(cppassist::string::encode(string.value(), cppassist::Encoding::UTF8));
    }
    else
    {
        const auto glyphs = font.value()->glyphs();

        auto text = std::u32string{};
        for (auto i = 0u; i < numChars.value(); ++i)
        {
            text += glyphs[std::rand() % glyphs.size()];
        }
        m_sequences.front().setString(text);
    }

    m_sequences.front().setWordWrap(wordWrap.value());
    m_sequences.front().setLineWidth(scaledLineWidth, scaledFontSize, *font.value());
    m_sequences.front().setAlignment(alignment.value());
    m_sequences.front().setLineAnchor(lineAnchor.value());

    m_sequences.front().setTransform(origin.value(), scaledFontSize, *font.value()
        , { viewport->z, viewport->w }, pixelPerInch.value(), margins.value());

    sequences.setValue(&m_sequences);
}
