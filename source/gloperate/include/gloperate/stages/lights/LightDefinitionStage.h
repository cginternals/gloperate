
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


class GLOPERATE_API LightDefinitionStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightDefinitionStage, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Stage that bundles information on a single light source into a definition object"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    Input<int> type;
    Input<glm::vec3> color;
    Input<glm::vec3> position;
    Input<glm::vec3> attenuationCoeffitients;

    Output<LightDefinition> light;

public:
    LightDefinitionStage(Environment * environment, const std::string & name = "LightDefinitionStage");

    virtual ~LightDefinitionStage();

protected:
    virtual void onProcess(AbstractGLContext * context) override;
};


} // namespace gloperate
