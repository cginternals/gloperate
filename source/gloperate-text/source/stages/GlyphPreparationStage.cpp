
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


void GlyphPreparationStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_vertexCloud = std::unique_ptr<GlyphVertexCloud>{ new GlyphVertexCloud{} };
}


void GlyphPreparationStage::onContextDeinit(gloperate::AbstractGLContext * context)
{
    m_vertexCloud = nullptr;
}


void GlyphPreparationStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    // get total number of glyphs
    auto numGlyphs = std::size_t{ 0 };
    for (const auto & sequence : *sequences.value())
    {
        numGlyphs += sequence.size(*font.value());
    }

    // prepare vertex cloud storage
    m_vertexCloud->vertices().clear();
    m_vertexCloud->vertices().resize(numGlyphs);

    // typeset and transform all sequences
    assert(font.value() != nullptr);

    auto vertexItr =m_vertexCloud->vertices().begin();
    for (const auto & sequence : *sequences.value())
    {
        /*auto extent = */Typesetter::typeset(sequence, *font.value(), vertexItr);
        vertexItr += sequence.size(*font.value());
    }

    if (optimized.value())
    {
        m_vertexCloud->optimize(*sequences.value(), *font.value()); // optimize and update drawable
    }
    else
    {
        m_vertexCloud->update(); // update drawable
    }

    m_vertexCloud->setTexture(font.value()->glyphTexture());

    vertexCloud.setValue(m_vertexCloud.get());
}


} // namespace gloperate_text
