#include <gloperate/stages/ColorGradientTextureStage.h>

#include <gloperate/base/ColorGradientList.h>

namespace gloperate
{

ColorGradientTextureStage::ColorGradientTextureStage()
{
    addInput("gradients", gradients);
    addInput("textureWidth", textureWidth);

    addOutput("gradientTexture", gradientTexture);
}

ColorGradientTextureStage::~ColorGradientTextureStage()
{
}

void ColorGradientTextureStage::process()
{
    gradientTexture.data() = gradients.data().generateTexture(textureWidth.data());

    invalidateOutputs();
}

} // namespace gloperate
