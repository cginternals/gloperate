
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <globjects/base/CachedValue.h>

#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractPerspectiveProjectionCapability
*/
class GLOPERATE_API PerspectiveProjectionCapability : public AbstractPerspectiveProjectionCapability
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewportCapability
    *    Viewport capability (must NOT be null!)
    */
    PerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PerspectiveProjectionCapability();

    // Virtual functions from AbstractPerspectiveProjectionCapability
    virtual float zNear() const override;
    virtual void setZNear(float zNear) override;
    virtual float zFar() const override;
    virtual void setZFar(float zFar) override;
    virtual float fovy() const override;
    virtual void setFovy(float fovy) override;
    virtual float aspectRatio() const override;

    // Virtual functions from AbstractProjectionCapability
    virtual const glm::mat4 & projection() const override;
    virtual const glm::mat4 & projectionInverted() const override;
    virtual glm::mat4 projectionForAspectRatio(float ratio) const override;

    /**
    *  @brief
    *    Update projection
    */
    void update() const;


protected:
    /**
    *  @brief
    *    Mark data dirty
    *
    *  @param[in] update
    *    Update matrices immediately (true), or later (false)?
    *
    *  @remarks
    *    After calling this function, the matrices will be recalculated the next time they're accessed.
    */
    void dirty(bool update = true);

    /**
    *  @brief
    *    Invalidate matrices, causing them to be recalculated
    */
    void invalidateMatrices() const;

    // Virtual functions from AbstractProjectionCapability
    virtual void setAspectRatio(float ratio) override;
    virtual void setAspectRatio(const glm::ivec2 & viewport) override;


protected:
    // Internal data
    mutable bool m_dirty;   /**< Has the data been changed? If true, matrices will be recalculated */

    // Projection data
    float m_fovy;           /**< Field-of-view angle (Y) */
    float m_aspect;         /**< Aspect ratio (width / height) */
    float m_zNear;          /**< Near plane */
    float m_zFar;           /**< Far plane */

    // Projection matrices
    globjects::CachedValue<glm::mat4> m_projection;         /**< Projection matrix */
    globjects::CachedValue<glm::mat4> m_projectionInverted; /**< Inverted projection matrix */
};


} // namespace gloperate
