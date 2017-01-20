
#include <gloperate/stages/multiframe/MultiFrameDiscDistributionStage.h>

#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace gloperate
{


CPPEXPOSE_COMPONENT(MultiFrameDiscDistributionStage, gloperate::Stage)


MultiFrameDiscDistributionStage::MultiFrameDiscDistributionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, isActive("isActive", this, true)
, radius("radius", this)
, currentMultiFrame("currentMultiFrame", this)
, value("value", this)
{
}

void MultiFrameDiscDistributionStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    if (!(*isActive))
    {
        value.setValue(glm::vec2(0.0f));

        return;
    }

    value.setValue(glm::diskRand(*radius));
}


} // namespace gloperate
