
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class MultiFrameDiscDistributionStage;
class DemoDOFCubeStage;


/**
*  @brief
*    Demo pipeline that renders a static cube onto the screen
*/
class GLOPERATE_API DemoDOFPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoDOFPipeline, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline that renders a static cube onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface; ///< Interface for rendering into a viewer


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
    DemoDOFPipeline(Environment * environment, const std::string & name = "DemoDOFPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoDOFPipeline();


protected:
    // Stages
    MultiFrameDiscDistributionStage * m_dofShiftStage;  ///< Stage generating DOF shift
    DemoDOFCubeStage                * m_cubeStage;      ///< Rendering stage
};


} // namespace gloperate
