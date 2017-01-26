
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class LightCreationStage;
class LightBufferTextureStage;
class LightTestStage;
class TimerStage;


/**
*  @brief
*    Pipeline that renders a rotating cube illuminated by 3 light sources
*/
class GLOPERATE_API LightTestPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightTestPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Stage that renders a rotating cube illuminated by 3 light sources"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    // Interfaces
    RenderInterface renderInterface;    ///< Interface for rendering into a viewer

    // Inputs
    Input<float> glossiness;            ///< Glossiness of the cube (0.0 to 1.0)

    Input<int> lightType1;              ///< Type of 1st light
    Input<int> lightType2;              ///< Type of 2nd light
    Input<int> lightType3;              ///< Type of 3rd light

    Input<glm::vec3> lightColor1;       ///< Color of 1st light
    Input<glm::vec3> lightColor2;       ///< Color of 2nd light
    Input<glm::vec3> lightColor3;       ///< Color of 3rd light

    Input<glm::vec3> lightPos1;         ///< Position/Direction of 1st light
    Input<glm::vec3> lightPos2;         ///< Position/Direction of 2nd light
    Input<glm::vec3> lightPos3;         ///< Position/Direction of 3rd light

    Input<glm::vec3> lightAttenuation1; ///< Attenuation coefficients of 1st light
    Input<glm::vec3> lightAttenuation2; ///< Attenuation coefficients of 2nd light
    Input<glm::vec3> lightAttenuation3; ///< Attenuation coefficients of 3rd light

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
    LightTestPipeline(Environment * environment, const std::string & name = "LightTestPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~LightTestPipeline();

protected:
    // Stages
    LightCreationStage         * m_lightDefStage1;
    LightCreationStage         * m_lightDefStage2;
    LightCreationStage         * m_lightDefStage3;
    LightBufferTextureStage    * m_lightAccumulationStage;
    TimerStage                 * m_timerStage;
    LightTestStage             * m_renderStage;
};


} // namespace gloperate
