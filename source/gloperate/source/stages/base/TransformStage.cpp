
#include <gloperate/stages/base/TransformStage.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TransformStage, gloperate::Stage)


TransformStage::TransformStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TransformStage", name)
, rotationAxis("rotationAxis", this, glm::vec3(0.0f, 1.0f, 0.0f))
, rotationAngle("rotationAngle", this, 0.0f)
, translation("translation", this, glm::vec3(0.0f, 0.0f, 0.0f))
, scale("scale", this, glm::vec3(1.0f, 1.0f, 1.0f))
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
    m_modelMatrix = glm::translate(m_modelMatrix, *translation);
    m_modelMatrix = glm::scale(m_modelMatrix, *scale);
    m_modelMatrix = glm::rotate(m_modelMatrix, *rotationAngle, *rotationAxis);

    // Update output matrix
    modelMatrix.setValue(m_modelMatrix);
}


} // namespace gloperate
