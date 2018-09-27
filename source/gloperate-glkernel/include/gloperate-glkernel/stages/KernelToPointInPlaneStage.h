
#pragma once


#include <vector>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that maintaines and provides information about multi frame aggregation progress
*/
class GLOPERATE_GLKERNEL_API KernelToPointInPlanestage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        KernelToPointInPlanestage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides subsequent points on a plane using a 2D kernel."
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<int>                      frameNumber;    ///< Current frame number
    Input<std::vector<glm::vec2> *> kernel;         ///< Kernel
    Input<glm::vec2>                kernelScale;    ///< Scaling of kernel (default: (1, 1))
    Input<glm::vec3>                planeReference; ///< Reference point on plane
    Input<glm::vec3>                planeNormal;    ///< Normal of plane

    // Outputs
    Output<glm::vec3>               position;      ///< Resulting point on plane


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
    KernelToPointInPlanestage(gloperate::Environment * environment, const std::string & name = "KernelToPointInPlanestage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~KernelToPointInPlanestage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;


};


} // namespace gloperate
