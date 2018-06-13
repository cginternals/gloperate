
#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class GLOPERATE_API CubeMapProjectionsStage : public gloperate::Stage
{
public:
    CubeMapProjectionsStage(gloperate::Environment * environment, const std::string & name = "CubeMapProjectionsStage");
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;


public:
    Input<glm::vec3> center;
    Input<float>     nearPlane;
    Input<float>     farPlane;

    Output<std::vector<glm::mat4>*> projections;
    Output<std::vector<glm::mat4>> projectionsDirect;


protected:
    virtual void onProcess() override;


protected:
    std::vector<glm::mat4> m_projections;
};


} // namespace gloperate
