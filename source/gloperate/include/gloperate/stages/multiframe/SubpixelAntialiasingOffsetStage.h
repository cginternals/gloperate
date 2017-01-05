#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace gloperate
{


/**
*  @brief
*    Stage that provides random offsets for subpixel antialiasing
*/
class GLOPERATE_API SubpixelAntialiasingOffsetStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SubpixelAntialiasingOffsetStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides random offsets for subpixel antialiasing"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<glm::vec4> viewport;      ///< Viewport, as vec4(xPos, yPos, width, height)
    Input<int> currentMultiFrame;   ///< Number of current frame

    // Outputs
    Output<glm::vec2> subPixelOffset; ///< Subpixel offset


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
    SubpixelAntialiasingOffsetStage(gloperate::Environment * environment, const std::string & name = "SubpixelAntialiasingOffsetStage");


protected:
    // Virtual stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
};


} // namespace gloperate
