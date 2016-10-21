
#include <gloperate/stages/lights/LightDefinitionStage.h>


namespace gloperate
{


LightDefinitionStage::LightDefinitionStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, type("type", this)
, color("color", this)
, position("position", this)
, attenuationCoeffitients("attenuationCoeffitients", this)
, light("light", this)
{
}

LightDefinitionStage::~LightDefinitionStage()
{
}

void LightDefinitionStage::onProcess(AbstractGLContext * context)
{
    light.setValue(LightDefinition{LightType(*type), *color, *position, *attenuationCoeffitients});
}


} // namespace gloperate
