
#include <gloperate-text/stages/GlyphPreparationStage.h>

#include <cassert>

#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>
#include <gloperate-text/Typesetter.h>


namespace gloperate_text
{


GlyphPreparationStage::GlyphPreparationStage()
: AbstractStage("GlyphPreparationStage")
{
    addInput("font", font);
    addInput("sequences", sequences);
    addInput("optimized", optimized);

    addOutput("vertexCloud", vertexCloud);
}

GlyphPreparationStage::~GlyphPreparationStage()
{
}

void GlyphPreparationStage::process()
{
    // get total number of glyphs
    auto numGlyphs = size_t(0u);
    for (const auto & sequence : sequences.data())
        numGlyphs += sequence.size(*font.data());

    // prepare vertex cloud storage
    auto & vc = vertexCloud.data();
    vc.vertices().clear();
    vc.vertices().resize(numGlyphs);

    // typeset and transform all sequences
    assert(font.data());

    auto index = vc.vertices().begin();
    for (const auto & sequence : sequences.data())
    {
        auto extent = Typesetter::typeset(sequence, *font.data(), index);
        index += sequence.size(*font.data());
    }

    if(optimized.data())
        vc.optimize(sequences.data(), *font.data()); // optimize and update drawable
    else
        vc.update(); // update drawable

    if(font.hasChanged())
        vc.setTexture(font.data()->glyphTexture());

    invalidateOutputs();
}


} // namespace gloperate_text
