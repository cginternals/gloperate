
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Timer.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that creates a constant timer
*
*  @remarks
*    This stage is part of the DemoPipeline
*/
class GLOPERATE_API DemoTimerStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoTimerStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Demo stage that creates a constant timer"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<float>  timeDelta;   ///< Time delta since last frame (in seconds)

    // Outputs
    Output<float> virtualTime; ///< Elapsed time (in seconds)


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
    DemoTimerStage(Environment * environment, const std::string & name = "DemoTimerStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoTimerStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;


protected:
    // Tools
    Timer m_timer;

    // Status
    float m_time;   ///< Virtual time (in seconds)
};


} // namespace gloperate
