
#include "GlyphSequenceDemoStage.h"

#include <cppassist/string/conversion.h>

#include <openll/Label.h>
#include <openll/FontFace.h>


GlyphSequenceDemoStage::GlyphSequenceDemoStage(gloperate::Environment * environment, const std::string & name)
: gloperate::Stage{ environment, "GlyphSequenceDemoStage", name }
, string      { "string",       this, "" }
, numChars    { "numChars",     this, 0 }
, font        { "font",         this, nullptr }
, fontSize    { "fontSize",     this, 1.0f }
, wordWrap    { "wordWrap",     this, true }
, lineWidth   { "lineWidth",    this, 400.0f }
, alignment   { "alignment",    this, openll::Alignment::Centered }
, lineAnchor  { "lineAnchor",   this, openll::LineAnchor::Baseline }
, origin      { "origin",       this, { 0.0f, 0.5f } }
, margins     { "margins",      this, { 0.0f, 0.0f, 0.0f, 0.0f } } // t r b l
, pixelPerInch{ "pixelPerInch", this, 72.0f }
, viewport    { "viewport",     this, { 0.0f, 0.0f, 0.0f, 0.0f } }
, sequences   { "sequences",    this }
{
}

void GlyphSequenceDemoStage::onProcess()
{
    m_sequences.resize(1);

    // TODO: property editor has a fixed range [0, 2 * PI], remove scaling once this is fixed!
    const auto scaledFontSize = fontSize.value() * 16.0f;
    const auto scaledLineWidth = lineWidth.value() * 160.0f * 7;

    if (numChars.value() == 0)
    {
        m_sequences.front().setText(cppassist::string::encode(string.value(), cppassist::Encoding::UTF8));
    }
    else
    {
        const auto glyphs = font.value()->glyphs();

        auto text = std::u32string{};
        for (auto i = 0u; i < numChars.value(); ++i)
        {
            text += glyphs[std::rand() % glyphs.size()];
        }
        m_sequences.front().setText(text);
    }

    sequences.setValue(&m_sequences);

    if (font.value() == nullptr)
    {
        return;
    }

    m_sequences.front().setWordWrap(wordWrap.value());
    m_sequences.front().setLineWidth(scaledLineWidth);
    m_sequences.front().setAlignment(alignment.value());
    m_sequences.front().setLineAnchor(lineAnchor.value());
    m_sequences.front().setFontSize(scaledFontSize);
    m_sequences.front().setFontFace(*font.value());
    m_sequences.front().setTransform2D(origin.value(), glm::uvec2{ viewport->z, viewport->w }, pixelPerInch.value());
    m_sequences.front().setMargins(margins.value());
}
