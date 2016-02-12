
#include "GlyphSequenceDemoStage.h"

#include <random>

#include <stringzeug/conversion.h>

#include <gloperate/painter/AbstractViewportCapability.h>

#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>


GlyphSequenceDemoStage::GlyphSequenceDemoStage()
: AbstractStage("GlyphSequenceDemoStage")
{
    addInput("string", string);
    addInput("numChars", numChars);
    addInput("font", font);
    addInput("fontSize", fontSize);
    addInput("pixelPerInch", pixelPerInch);
    addInput("viewport", viewport);
    addInput("origin", origin);
    addInput("margins", margins);
    addInput("wordWrap", wordWrap);
    addInput("lineWidth", lineWidth);
    addInput("alignment", alignment);
    addInput("lineAnchor", lineAnchor);

    addOutput("sequences", sequences);
}

GlyphSequenceDemoStage::~GlyphSequenceDemoStage()
{
}

void GlyphSequenceDemoStage::process()
{
    sequences.data().clear();
    sequences.data().resize(1);

    //font.data()->setLinespace(1.25f);

    if(numChars.data() == 0)
        sequences.data()[0].setString(stringzeug::encode(string.data(), stringzeug::Encoding::UTF8));
    else
    {
        const auto glyphs = font.data()->glyphs();

        auto s = std::u32string();
        for (auto i = 0u; i < numChars.data(); ++i)
        {
            s += glyphs[std::rand() % glyphs.size()];
        }
        sequences.data()[0].setString(s);
    }

    sequences.data()[0].setWordWrap(wordWrap.data());
    sequences.data()[0].setLineWidth(lineWidth.data(), fontSize.data(), *font.data());
    sequences.data()[0].setAlignment(alignment.data());
    sequences.data()[0].setLineAnchor(lineAnchor.data());

    sequences.data()[0].setTransform(origin.data(), fontSize.data(), *font.data()
        , { viewport.data()->width(), viewport.data()->height() }, pixelPerInch.data(), margins.data());

    //sequences.data()[0].setTransform(origin.data(), fontSize.data(), *font.data()
    //    , { viewport.data()->width(), viewport.data()->height() });

    invalidateOutputs();
}
