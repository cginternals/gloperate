
#include <gloperate/stages/base/TransformStage.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TransformStage, gloperate::Stage)


TransformStage::TransformStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TransformStage", name)
, angle      ("angle",       this, 0.0f)
, modelMatrix("modelMatrix", this)
{
}

TransformStage::~TransformStage()
{
}

void TransformStage::onProcess()
{
    // Calculate model matrix
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::rotate(m_modelMatrix, *angle, glm::vec3(0.0, 1.0, 0.0));

    // Update output matrix
    modelMatrix.setValue(m_modelMatrix);
}


} // namespace gloperate
