#include <gloperate/stages/ColorGradientSelectionStage.h>

#include <gloperate/base/ColorGradientList.h>
#include <gloperate/base/AbstractColorGradient.h>

namespace gloperate
{

ColorGradientSelectionStage::ColorGradientSelectionStage()
{
    addInput("gradients", gradients);
    addInput("gradientName", gradientName);

    addOutput("gradientIndex", gradientIndex);
}

ColorGradientSelectionStage::~ColorGradientSelectionStage()
{
}

void ColorGradientSelectionStage::process()
{
    gradientIndex.data() = gradients.data().indexOf(gradientName.data());
    gradient.data() = gradients.data().at(gradientName.data());

    invalidateOutputs();
}

} // namespace gloperate
