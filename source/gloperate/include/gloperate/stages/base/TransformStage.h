
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that creates a transformation matrix
*/
class GLOPERATE_API TransformStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TransformStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Stage that creates a transformation matrix"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<float> angle; ///< Rotation angle (in radians)

    // Outputs
    Output<glm::mat4> modelMatrix; ///< Transformation matrix


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
    TransformStage(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TransformStage();


protected:
    // Virtual Stage functions
    virtual void onProcess() override;


protected:
    // Rendering objects
    glm::mat4 m_modelMatrix; ///< Current model matrix
};


} // namespace gloperate
