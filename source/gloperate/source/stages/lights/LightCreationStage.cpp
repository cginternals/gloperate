
#include <gloperate/stages/lights/LightCreationStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(LightCreationStage, gloperate::Stage)


LightCreationStage::LightCreationStage(Environment * environment, const std::string & name)
: Stage(environment, "LightCreationStage", name)
, type("type", this)
, color("color", this)
, position("position", this)
, attenuationCoefficients("attenuationCoefficients", this)
, light("light", this)
{
}

LightCreationStage::~LightCreationStage()
{
}

void LightCreationStage::onProcess()
{
    light.setValue(Light{*type, (*color).toVec3(), *position, *attenuationCoefficients});
}


} // namespace gloperate
