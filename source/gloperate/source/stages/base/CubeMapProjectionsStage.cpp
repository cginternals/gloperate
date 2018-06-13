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
, projections("projections", this)
, projectionsDirect("projectionsDirect", this)
{
    m_projections.resize(6);
}

void CubeMapProjectionsStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
}

void CubeMapProjectionsStage::onProcess()
{
    const auto c = *center;
    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, *nearPlane, *farPlane);

    for (int i = 0; i < 6; ++i)
    {
        m_projections.at(i) = projection * glm::lookAt(c, c + viewDirs[i], ups[i]);
    }

    projections.setValue(&m_projections);
    projectionsDirect.setValue(m_projections);
}

} // namespace gloperate
