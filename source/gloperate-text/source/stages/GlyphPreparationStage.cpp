
#include <gloperate-text/stages/GlyphPreparationStage.h>

#include <cassert>

#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>
#include <gloperate-text/Typesetter.h>


namespace gloperate_text
{


GlyphPreparationStage::GlyphPreparationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, font("font", this)
, sequences("sequences", this)
, optimized("optimized", this)
, vertexCloud("vertexCloud", this)
{
}

GlyphPreparationStage::~GlyphPreparationStage()
{
}

void GlyphPreparationStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    // get total number of glyphs
    auto numGlyphs = size_t(0u);
    for (const auto & sequence : (*sequences))
        numGlyphs += sequence.size(*(*font));

    // prepare vertex cloud storage
    auto & vc = (*vertexCloud);
    vc.vertices().clear();
    vc.vertices().resize(numGlyphs);

    // typeset and transform all sequences
    assert((*font));

    auto index = vc.vertices().begin();
    for (const auto & sequence : (*sequences))
    {
        /*auto extent = */Typesetter::typeset(sequence, *(*font), index);
        index += sequence.size(*(*font));
    }

    if((*optimized))
    {
        vc.optimize((*sequences), *(*font)); // optimize and update drawable
    }
    else
    {
        vc.update(); // update drawable
    }

    vc.setTexture((*font)->glyphTexture());

    // invalidateOutputs(); // TODO: check if outputs are set to valid
}


} // namespace gloperate_text
