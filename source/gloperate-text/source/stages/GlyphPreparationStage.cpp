
#include <gloperate-text/stages/GlyphPreparationStage.h>

#include <cassert>

#include <chrono>
#include <iostream>
#include <fstream>

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

    std::ofstream glyphlog;
    glyphlog.open("glyph.log", std::ios::out | std::ios::ate);

    auto N = 1;
    auto index = vc.vertices().begin();

    const auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i)
    {
        index = vc.vertices().begin();
        for (const auto & sequence : sequences.data())
        {
            auto extent = Typesetter::typeset(sequence, *font.data(), index);
            index += sequence.size(*font.data());

            glyphlog << "extent:    " << extent.x << " " << extent.y << std::endl;
        }
    }

    const auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i)
    {
        vc.update(); // update drawable
        vc.setTexture(font.data()->glyphTexture());
    }

    const auto t2 = std::chrono::high_resolution_clock::now();

    using milli = std::chrono::duration<float, std::milli>;

    glyphlog << "typeset:   " << std::chrono::duration_cast<milli>(t1 - t0).count() / static_cast<float>(N) << std::endl;
    glyphlog << "update:    " << std::chrono::duration_cast<milli>(t2 - t1).count() / static_cast<float>(N) << std::endl << std::endl;
    glyphlog.flush();
    glyphlog.close();

    invalidateOutputs();
}


} // namespace gloperate_text
