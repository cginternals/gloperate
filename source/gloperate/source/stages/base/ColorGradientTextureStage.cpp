
#include <gloperate/stages/base/ColorGradientTextureStage.h>

#include <gloperate/base/ColorGradientList.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientTextureStage, gloperate::Stage)


ColorGradientTextureStage::ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientTextureStage", name)
, gradients("gradients", this)
, textureWidth("textureWidth", this, 128)
, gradientTexture("gradientTexture", this)
{
}

void ColorGradientTextureStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    gradientTexture.setValue(gradients->generateTexture(*textureWidth));
}


} // namespace gloperate
