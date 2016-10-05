
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace gloperate {
class DemoTimerStage;
class DemoTriangleStage;
} // namespace gloperate


namespace gloperate_glheadless {


class DemoOffscreenStage;


/**
*  @brief
*    Demo pipeline displaying a rotating texture created on an offscreen context
*/
class GLOPERATE_GLHEADLESS_API DemoOffscreenPipeline : public gloperate::Pipeline {
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoOffscreenPipeline, gloperate::Stage
        , "RenderStage"   // Tags
        , ""              // Icon
        , ""              // Annotations
        , "Demo pipeline displaying a rotating texture created on an offscreen context"
        , GLOPERATE_AUTHOR_ORGANIZATION
        , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer


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
    DemoOffscreenPipeline(gloperate::Environment * environment, const std::string & name = "DemoOffscreenPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoOffscreenPipeline();


protected:
    // Stages
    DemoOffscreenStage           * m_offscreenStage; ///< Offscreen stage to render the texture
    gloperate::DemoTimerStage    * m_timerStage;     ///< Timer stage to drive rotation
    gloperate::DemoTriangleStage * m_triangleStage;  ///< Render stage to display the rotating texture
};


} // namespace gloperate_glheadless
