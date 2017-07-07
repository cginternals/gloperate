
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
*    Stage that adjusts the viewport by a given scaleFactor
*/
class GLOPERATE_API VirtualViewportStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        VirtualViewportStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that adjusts the viewport by a given scaleFactor"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<glm::vec4>  viewport;     ///< old viewport
    Input<float>      scaleFactor;  ///< Scale factor

    // Outputs
    Output<glm::vec4> virtualViewport; ///< new viewport

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
    VirtualViewportStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VirtualViewportStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;


protected:
    glm::vec4 m_virtualViewport;
};


} // namespace gloperate
