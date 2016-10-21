
#pragma once


#include <glm/vec3.hpp>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/rendering/LightDefinition.h>


namespace gloperate
{


class GLOPERATE_API LightDefinitionStage : public Stage
{
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
