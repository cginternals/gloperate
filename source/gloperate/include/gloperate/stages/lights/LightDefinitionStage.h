
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec3.hpp>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/rendering/LightDefinition.h>


namespace gloperate
{


/**
*  @brief
*    Stage that bundles information on a single light source into a Light object
*/
class GLOPERATE_API LightDefinitionStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightDefinitionStage, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Stage that bundles information on a single light source into a Light object"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    // Inputs
    Input<int> type;                          ///< Type id of the light
    Input<glm::vec3> color;                   ///< Color of the light
    Input<glm::vec3> position;                ///< Position of local light, direction of global light
    Input<glm::vec3> attenuationCoeffitients; ///< Coeffitients for attenuation in the order: constant, linear, quadratic

    // Output
    Output<LightDefinition> light;                      ///< Light object

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
    LightDefinitionStage(Environment * environment, const std::string & name = "LightDefinitionStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~LightDefinitionStage();

protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;
};


} // namespace gloperate
