#include <gloperate/stages/base/CubeMapProjectionsStage.h>

#include <glm/gtc/matrix_transform.hpp>


namespace
{

const glm::vec3 viewDirs[] = { glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1) };
const glm::vec3 ups[] = { glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0) };

} // namespace


namespace gloperate
{

CubeMapProjectionsStage::CubeMapProjectionsStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, center("center", this, glm::vec3(0.0f, 0.0f, 0.0f))
, nearPlane("nearPlane", this, 0.1f)
, farPlane("farPlane", this, 4.0f)
, projections("projections", this, nullptr)
{
    m_projections.resize(6);
}

void CubeMapProjectionsStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
}

void CubeMapProjectionsStage::onProcess()
{
    const auto c = *center;

    for (int i = 0; i < 6; ++i)
    {
        const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, *nearPlane, *farPlane);
        const auto view = glm::lookAt(c, c + viewDirs[i], ups[i]);

        projections->at(i) = std::move(projection) * std::move(view);
    }

    projections.setValue(&m_projections);
}

} // namespace gloperate
