
#include <gloperate/stages/base/ColorGradientSelectionStage.h>

#include <gloperate/base/AbstractColorGradient.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientSelectionStage, gloperate::Stage)


ColorGradientSelectionStage::ColorGradientSelectionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, gradients("gradients", this)
, gradientName("gradientName", this)
, gradient("gradient", this)
, gradientIndex("gradientIndex", this)
{
}

void ColorGradientSelectionStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    gradientIndex.setValue((*gradients).indexOf(*gradientName));
    gradient.setValue((*gradients).at(*gradientName));
}


} // namespace gloperate
