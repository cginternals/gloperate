
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/base/CachedValue.h>

#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/OrthographicProjectionCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>


/**
*  @brief
*    Default implementation for AbstractOrthographicProjectionCapability
*/

namespace gloperate{

class GLOPERATE_API CombinedProjectionCapability : public gloperate::AbstractProjectionCapability
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewportCapability
    *    Viewport capability (must NOT be null!)
    */
    CombinedProjectionCapability(gloperate::AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~CombinedProjectionCapability();

    // Virtual functions from AbstractProjectionCapability
    virtual float zNear() const override;
    virtual void setZNear(float zNear) override;
    virtual float zFar() const override;
    virtual void setZFar(float zFar) override;
    virtual float aspectRatio() const override;
    
    virtual const glm::mat4 & projection() const override;
    virtual const glm::mat4 & projectionInverted() const override;
    virtual glm::mat4 projectionForAspectRatio(float ratio) const override;

    /**
    *  @brief
    *    Update projection
    */
    void update() const;
    
    float mix() const;
    void setMix(float mix);
    void setOrthoFOV(const glm::vec3 & eye, const glm::vec3 & focus);


protected:
    // Virtual functions from AbstractProjectionCapability
    virtual void setAspectRatio(float ratio) override;
    virtual void setAspectRatio(const glm::ivec2 & viewport) override;
    
    glm::mat4 interpolate(const glm::mat4& first, const glm::mat4& second) const;


protected:
    gloperate::OrthographicProjectionCapability m_orthoCapability;
    gloperate::PerspectiveProjectionCapability m_perspectiveCapability;
    
    float m_mix;
    
    glm::mat4 m_projection;
    glm::mat4 m_invertedProjection;
};

} // namespace gloperate
