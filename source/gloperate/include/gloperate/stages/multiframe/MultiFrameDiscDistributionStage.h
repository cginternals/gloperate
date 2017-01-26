#pragma once

#include <glm/vec2.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>


namespace gloperate
{


/**
*  @brief
*    Stage that provides random vectors with disc distribution
*/
class GLOPERATE_API MultiFrameDiscDistributionStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameDiscDistributionStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides random vectors with disc distribution"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<bool> isActive;         ///< Generate new vector
    Input<float> radius;          ///< Maximum length of vector
    Input<int> currentMultiFrame; ///< Number of currently aggregated frame

    // Outputs
    Output<glm::vec2> value;      ///< Random vector


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
    MultiFrameDiscDistributionStage(gloperate::Environment * environment, const std::string & name = "MultiFrameDiscDistributionStage");


protected:
    // Virtual Stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
};


} // namespace gloperate
