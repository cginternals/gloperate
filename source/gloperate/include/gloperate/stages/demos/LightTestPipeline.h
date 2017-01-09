
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class LightDefinitionStage;
class LightDataAccumulationStage;
class LightTestStage;
class TimerStage;


class GLOPERATE_API LightTestPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightTestPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Stage that renders a rotating cube using provided lighting information"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    RenderInterface renderInterface;

    Input<float> glossiness;

    Input<int> lightType1;
    Input<int> lightType2;
    Input<int> lightType3;

    Input<glm::vec3> lightColor1;
    Input<glm::vec3> lightColor2;
    Input<glm::vec3> lightColor3;

    Input<glm::vec3> lightPos1;
    Input<glm::vec3> lightPos2;
    Input<glm::vec3> lightPos3;

    Input<glm::vec3> lightAttenuation1;
    Input<glm::vec3> lightAttenuation2;
    Input<glm::vec3> lightAttenuation3;

public:
    LightTestPipeline(Environment * environment, const std::string & name = "LightTestPipeline");
    virtual ~LightTestPipeline();

protected:
    LightDefinitionStage       * m_lightDefStage1;
    LightDefinitionStage       * m_lightDefStage2;
    LightDefinitionStage       * m_lightDefStage3;
    LightDataAccumulationStage * m_lightAccumulationStage;
    TimerStage                 * m_timerStage;
    LightTestStage             * m_renderStage;
};


} // namespace gloperate
