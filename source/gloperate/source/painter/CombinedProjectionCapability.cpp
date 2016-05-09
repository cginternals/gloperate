
#include <gloperate/painter/CombinedProjectionCapability.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate{

CombinedProjectionCapability::CombinedProjectionCapability(gloperate::AbstractViewportCapability *viewportCapability) 
: gloperate::AbstractProjectionCapability(viewportCapability)
, m_orthoCapability(viewportCapability)
, m_perspectiveCapability(viewportCapability)
, m_mix(1.0f)
{ 
}  

CombinedProjectionCapability::~CombinedProjectionCapability()
{
}

float CombinedProjectionCapability::zNear() const
{
    return glm::mix(m_orthoCapability.zNear(), m_perspectiveCapability.zNear(), m_mix);
}

void CombinedProjectionCapability::setZNear(const float zNear)
{
    m_orthoCapability.setZNear(zNear);
    m_perspectiveCapability.setZNear(zNear);
    update();

    m_projection = interpolate(m_orthoCapability.projection(), m_perspectiveCapability.projection());
    m_invertedProjection = glm::inverse(m_projection);
}

float CombinedProjectionCapability::zFar() const
{
    return glm::mix(m_orthoCapability.zFar(), m_perspectiveCapability.zFar(), m_mix);
}

void CombinedProjectionCapability::setZFar(const float zFar)
{
    m_orthoCapability.setZFar(zFar);
    m_perspectiveCapability.setZFar(zFar);
    update();

    m_projection = interpolate(m_orthoCapability.projection(), m_perspectiveCapability.projection());
    m_invertedProjection = glm::inverse(m_projection);
}

float CombinedProjectionCapability::aspectRatio() const
{
    return m_orthoCapability.aspectRatio();
}

const glm::mat4 & CombinedProjectionCapability::projection() const
{  
    return m_projection;
}

const glm::mat4 & CombinedProjectionCapability::projectionInverted() const
{
    return m_invertedProjection;
}

glm::mat4 CombinedProjectionCapability::projectionForAspectRatio(float ratio) const
{
    return interpolate(m_orthoCapability.projectionForAspectRatio(ratio), m_perspectiveCapability.projectionForAspectRatio(ratio));
}

void CombinedProjectionCapability::update() const
{
    m_orthoCapability.update();
    m_perspectiveCapability.update();
}

void CombinedProjectionCapability::setAspectRatio(float ratio)
{
    m_orthoCapability.setAspectRatio(ratio);
    m_perspectiveCapability.setAspectRatio(ratio);
}

void CombinedProjectionCapability::setAspectRatio(const glm::ivec2 & viewport)
{
    m_orthoCapability.setAspectRatio(viewport);
    m_orthoCapability.setAspectRatio(viewport);
}

void CombinedProjectionCapability::setMix(float mix)
{
    m_mix = mix;
    m_projection = interpolate(m_orthoCapability.projection(), m_perspectiveCapability.projection());
    m_invertedProjection = glm::inverse(m_projection);
}

float CombinedProjectionCapability::mix() const
{
    return m_mix;
}

glm::mat4 CombinedProjectionCapability::interpolate(const glm::mat4 &first, const glm::mat4 &second) const
{
    return glm::mix(first, second, m_mix);
}

void CombinedProjectionCapability::setOrthoFOV(float z)
{
    auto alpha = m_perspectiveCapability.fovy();
    auto fovy = glm::tan(alpha) * z * 0.9f;
    m_orthoCapability.setHeight(std::max(fovy, 0.1f));
}

} // namespace gloperate
