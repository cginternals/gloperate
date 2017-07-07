
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>



namespace gloperate
{

/**
*  @brief
*    Stage that scales a viewport by a given scaleFactor
*/
class GLOPERATE_API ViewportScaleStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ViewportScaleStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that adjusts the viewport by a given scaleFactor"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<glm::vec4>  viewport;     ///< Input viewport
    Input<float>      scaleFactor;  ///< Scale factor

    // Outputs
    Output<glm::vec4> scaledViewport; ///< Scaled viewport


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
    ViewportScaleStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewportScaleStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
};


} // namespace gloperate
