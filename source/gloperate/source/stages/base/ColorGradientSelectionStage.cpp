
#include <gloperate/stages/base/ColorGradientSelectionStage.h>

#include <gloperate/rendering/AbstractColorGradient.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientSelectionStage, gloperate::Stage)


ColorGradientSelectionStage::ColorGradientSelectionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientSelectionStage", name)
, gradients("gradients", this)
, gradientName("gradientName", this)
, gradient("gradient", this)
, gradientIndex("gradientIndex", this)
{
}

ColorGradientSelectionStage::~ColorGradientSelectionStage()
{
}

void ColorGradientSelectionStage::onProcess()
{
    // Update ooutputs
    gradientIndex.setValue(gradients->indexOf(*gradientName));
    gradient.setValue(gradients->at(*gradientName));
}


} // namespace gloperate
