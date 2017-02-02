
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

void LightCreationStage::onProcess(AbstractGLContext * context)
{
    light.setValue(Light{LightType(*type), *color, *position, *attenuationCoefficients});
}


} // namespace gloperate
