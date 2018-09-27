
#pragma once


#include <glm/vec4.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that maintaines and provides information about multi frame aggregation progress
*/
class GLOPERATE_GLKERNEL_API MultiFrameControlStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameControlStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that maintaines and provides information about multi frame aggregation progress"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<float>     timeDelta;         ///< Passed time in seconds since last frame
    Input<glm::vec4> viewport;          ///< the viewport to restart aggregation
    Input<int>       frameNumber;       ///< Total frame count
    Input<int>       multiFrameCount;   ///< Maximum number of frames to aggregate

    // Outputs
    Output<int>      currentFrame;      ///< Number of currently aggregated frame
    Output<float>    aggregationFactor; ///< Weight for aggregating the current frame (= 1 / currentFrame)


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
    MultiFrameControlStage(gloperate::Environment * environment, const std::string & name = "MultiFrameControlStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameControlStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onInputValueChanged(gloperate::AbstractSlot * slot) override;


protected:
    // Data
    int m_currentFrame; ///< Number of currently aggregated frame
};


} // namespace gloperate_glkernel
