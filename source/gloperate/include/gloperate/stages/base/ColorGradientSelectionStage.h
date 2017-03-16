
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/base/ColorGradientList.h>


namespace gloperate
{


class AbstractColorGradient;


/**
*  @brief
*    Stage that selects a gradient from a list based on a given name
*/
class GLOPERATE_API ColorGradientSelectionStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientSelectionStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that selects a gradient from a list based on a given name"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    Input<ColorGradientList *>            gradients;     ///< List of gradients
    Input<std::string>                    gradientName;  ///< Name of chosen gradient

    Output<const AbstractColorGradient *> gradient;      ///< Gradient
    Output<size_t>                        gradientIndex; ///< Index of gradient


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    ColorGradientSelectionStage(gloperate::Environment * environment, const std::string & name = "");


protected:
    /**
    *  @brief
    *    Process one iteration
    *
    *  @remarks
    *    Overriden
    */
    virtual void onProcess(gloperate::AbstractGLContext * /*context*/) override;
};


} // namespace gloperate
