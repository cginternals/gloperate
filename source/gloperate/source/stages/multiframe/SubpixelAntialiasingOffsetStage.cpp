
#include <gloperate/stages/multiframe/SubpixelAntialiasingOffsetStage.h>

#include <glm/gtc/random.hpp>


namespace gloperate
{


SubpixelAntialiasingOffsetStage::SubpixelAntialiasingOffsetStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, viewport("viewport", this)
, currentMultiFrame("currentMultiFrame", this)
, multiFrameCount("multiFrameCount", this)
, subPixelOffset("subPixelOffset", this)
{
}

void SubpixelAntialiasingOffsetStage::onProcess(gloperate::AbstractGLContext * context)
{
    (*subPixelOffset) = glm::diskRand(1.0f) / glm::vec2((*viewport).z, (*viewport).w);

    // invalidateOutputs();
}


} // namespace gloperate
