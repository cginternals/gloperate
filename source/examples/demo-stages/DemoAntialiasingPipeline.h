
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <demo-stages/demo-stages_api.h>


namespace gloperate
{

class SubpixelAntialiasingOffsetStage;

}


class DemoAntialiasableTriangleStage;


/**
*  @brief
*    Demo pipeline that renders a static triangle onto the screen
*/
class DEMO_STAGES_API DemoAntialiasingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoAntialiasingPipeline, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline that renders a static triangle onto the screen"
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
    *    Environment to which the pipeline belongs (must NOT be null!)
    *  @param[in] name
    *    Pipeline name
    */
    DemoAntialiasingPipeline(gloperate::Environment * environment, const std::string & name = "DemoAntialiasingPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoAntialiasingPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate::SubpixelAntialiasingOffsetStage> m_subpixelStage; ///< Stage generating subpixel offset for antialiasing
    std::unique_ptr<DemoAntialiasableTriangleStage>             m_triangleStage; ///< Rendering stage
};
