
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


class AbstractColorGradient;
class ColorGradientList;


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
    // Inputs
    Input<ColorGradientList *>            gradients; ///< List of color gradients
    Input<std::string>                    name;      ///< Name of chosen gradient

    // Outputs
    Output<const AbstractColorGradient *> gradient;  ///< Color gradient
    Output<unsigned int>                  index;     ///< Index of color gradient


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

    /**
    *  @brief
    *    Destructor
    */
    ~ColorGradientSelectionStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
};


} // namespace gloperate
