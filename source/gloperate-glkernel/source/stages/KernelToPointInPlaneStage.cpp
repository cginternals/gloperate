
#include <gloperate-glkernel/stages/KernelToPointInPlaneStage.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>

#include <glm/gtc/matrix_transform.hpp>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(KernelToPointInPlanestage, gloperate::Stage)


KernelToPointInPlanestage::KernelToPointInPlanestage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, frameNumber("frameNumber", this, 0)
, kernel("kernel", this, nullptr)
, kernelScale("kernelScale", this, glm::vec2(1.0f, 1.0f))
, planeReference("planeReference", this, glm::vec3(0.0f, 0.0f, 0.0f))
, planeNormal("planeNormal", this, glm::vec3(0.0f, 1.0f, 0.0f))
, position("position", this, glm::vec3(0.0f, 0.0f, 0.0f))
{
}

KernelToPointInPlanestage::~KernelToPointInPlanestage()
{
}

void KernelToPointInPlanestage::onProcess()
{
    if (*kernel == nullptr)
    {
        position.setValue(*planeReference);

        return;
    }

    auto center = *planeReference;
    auto currentOffset = glm::vec2(0.0f, 0.0f);
    const auto normal = glm::normalize((*planeNormal));

    if (static_cast<unsigned int>(*frameNumber) < kernel.value()->size())
    {
        kernel.value()->at(*frameNumber);
        currentOffset *= *kernelScale;
    }

    if (glm::dot(normal, normal) <= 0.0f || glm::dot(currentOffset, currentOffset) > 0.0f)
    {
        position.setValue(*planeReference);

        return;
    }

    if (glm::dot(normal, glm::vec3(0.0f, 1.0f, 0.0f)) > 0.99f)
    {
        const auto side = glm::cross(normal, glm::normalize(normal + glm::vec3(1.0f, 0.f, 0.0f)));
        const auto up = glm::cross(normal, side);

        center = center + side * currentOffset.x + up * currentOffset.y;
    }
    else
    {
        const auto side = glm::cross(normal, glm::normalize(normal + glm::vec3(0.0f, 1.f, 0.0f)));
        const auto up = glm::cross(normal, side);

        center = center + side * currentOffset.x + up * currentOffset.y;
    }

    position.setValue(center);
}


} // namespace gloperate
