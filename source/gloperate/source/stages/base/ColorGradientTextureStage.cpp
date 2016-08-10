
#include <gloperate/stages/base/ColorGradientTextureStage.h>

#include <gloperate/base/ColorGradientList.h>

namespace gloperate
{

ColorGradientTextureStage::ColorGradientTextureStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, gradients("gradients", this)
, textureWidth("textureWidth", this)
, gradientTexture("gradientTexture", this)
{
}

void ColorGradientTextureStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    gradientTexture.setValue((*gradients).generateTexture((*textureWidth)));
}

} // namespace gloperate
