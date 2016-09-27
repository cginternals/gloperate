
#include <gloperate/stages/multiframe/MultiFrameDiscDistributionStage.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace gloperate
{


CPPEXPOSE_COMPONENT(MultiFrameDiscDistributionStage, gloperate::Stage)


MultiFrameDiscDistributionStage::MultiFrameDiscDistributionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, isActive("isActive", this)
, radius("radius", this)
, currentMultiFrame("currentMultiFrame", this)
, multiFrameCount("multiFrameCount", this)
, value("value", this)
{
}

void MultiFrameDiscDistributionStage::onProcess(gloperate::AbstractGLContext * context)
{
    if (isActive.isConnected() && !(*isActive))
    {
        (*value) = glm::vec2(0.0f);

        // invalidateOutputs();

        return;
    }

    (*value) = glm::diskRand((*radius));

    // invalidateOutputs();
}


} // namespace gloperate
