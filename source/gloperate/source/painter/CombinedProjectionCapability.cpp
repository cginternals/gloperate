
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

    invalidateMatrices();
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

    invalidateMatrices();
}

float CombinedProjectionCapability::aspectRatio() const
{
    return m_orthoCapability.aspectRatio();
}

const glm::mat4 & CombinedProjectionCapability::projection() const
{  
    if(!m_projection.isValid())
    {
        m_projection.setValue(interpolate(m_orthoCapability.projection(), m_perspectiveCapability.projection()));
    }
    return m_projection.value();
}

const glm::mat4 & CombinedProjectionCapability::projectionInverted() const
{
    if(!m_invertedProjection.isValid())
    {
        m_invertedProjection.setValue(glm::inverse(projection()));
    }
    return m_invertedProjection.value();
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
    
    invalidateMatrices();
}

void CombinedProjectionCapability::setAspectRatio(const glm::ivec2 & viewport)
{
    m_orthoCapability.setAspectRatio(viewport);
    m_perspectiveCapability.setAspectRatio(viewport);
    
    invalidateMatrices();
}

void CombinedProjectionCapability::setMix(float mix)
{
    m_mix = mix;
    
    invalidateMatrices();
}

float CombinedProjectionCapability::mix() const
{
    return m_mix;
}

glm::mat4 CombinedProjectionCapability::interpolate(const glm::mat4 &first, const glm::mat4 &second) const
{
    return glm::mix(first, second, m_mix);
}

void CombinedProjectionCapability::setOrthoFOV(const glm::vec3 & eye, const glm::vec3 & focus)
{
    static const float offsetSmoothing = 0.85;
    auto alpha = m_perspectiveCapability.fovy();
    auto fovy = glm::tan(alpha) * glm::length(eye-focus) * offsetSmoothing;
    m_orthoCapability.setHeight(fovy);
}

void CombinedProjectionCapability::invalidateMatrices()
{
    m_projection.invalidate();
    m_invertedProjection.invalidate();
}

} // namespace gloperate
