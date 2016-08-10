
#pragma once

#include <string>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/ColorGradientList.h>


namespace gloperate
{


class AbstractColorGradient;


} // namespace gloperate


namespace gloperate
{

class GLOPERATE_API ColorGradientSelectionStage : public gloperate::Stage
{
public:
    ColorGradientSelectionStage(gloperate::Environment * environment, const std::string & name = "ColorGradientSelectionStage");

public:
    Input<ColorGradientList> gradients;
    Input<std::string> gradientName;

    Output<const AbstractColorGradient *> gradient;
    Output<size_t> gradientIndex;

protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
};

} // namespace gloperate
