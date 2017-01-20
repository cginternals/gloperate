
#include <gloperate/stages/multiframe/SubpixelAntialiasingOffsetStage.h>

#include <glm/gtc/random.hpp>


namespace gloperate
{


CPPEXPOSE_COMPONENT(SubpixelAntialiasingOffsetStage, gloperate::Stage)


SubpixelAntialiasingOffsetStage::SubpixelAntialiasingOffsetStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, viewport("viewport", this)
, currentMultiFrame("currentMultiFrame", this)
, subPixelOffset("subPixelOffset", this)
{
}

void SubpixelAntialiasingOffsetStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    subPixelOffset.setValue(glm::diskRand(1.0f) / glm::vec2((*viewport).z, (*viewport).w));
}


} // namespace gloperate
