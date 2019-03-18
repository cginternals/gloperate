
#include <gloperate-text/stages/GlyphPreparationStage.h>

#include <glm/vec2.hpp>

#include <openll/FontFace.h>
#include <openll/Typesetter.h>
#include <openll/GlyphVertexCloud.h>


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

void GlyphPreparationStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_vertexCloud = cppassist::make_unique<openll::GlyphVertexCloud>();

    vertexCloud.invalidate();
}

void GlyphPreparationStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_vertexCloud = nullptr;

    vertexCloud.setValue(nullptr);
}

void GlyphPreparationStage::onProcess()
{
    openll::Typesetter::typeset(*m_vertexCloud.get(), *sequences.value(), optimized.value(), false);

    m_vertexCloud->update(); // update drawable
    m_vertexCloud->setTexture(font.value()->glyphTexture());

    vertexCloud.setValue(m_vertexCloud.get());
}


} // namespace gloperate_text
