
#include <gloperate/stages/base/ColorGradientSelectionStage.h>

#include <gloperate/rendering/AbstractColorGradient.h>

#include <gloperate/rendering/ColorGradientList.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientSelectionStage, gloperate::Stage)


ColorGradientSelectionStage::ColorGradientSelectionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "ColorGradientSelectionStage", name)
, gradients("gradients", this)
, name("name", this)
, gradient("gradient", this)
, index("index", this)
{
}

ColorGradientSelectionStage::~ColorGradientSelectionStage()
{
}

void ColorGradientSelectionStage::onProcess()
{
    // Update outputs
    index.setValue(gradients->indexOf(*name));
    gradient.setValue(gradients->at(*name));
}


} // namespace gloperate
