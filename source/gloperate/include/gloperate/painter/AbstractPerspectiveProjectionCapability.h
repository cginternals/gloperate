
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractProjectionCapability.h>


namespace gloperate
{


/**
*  @brief
*    Capability to access a perspective camera projection
*/
class GLOPERATE_API AbstractPerspectiveProjectionCapability : public AbstractProjectionCapability
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewportCapability
    *    Viewport capability (must NOT be null!)
    */
    AbstractPerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractPerspectiveProjectionCapability();

    /**
    *  @brief
    *    Get near plane
    *
    *  @return
    *    Near plane
    */
    virtual float zNear() const override = 0;

    /**
    *  @brief
    *    Set near plane
    *
    *  @param[in] zNear
    *    Near plane
    */
    virtual void setZNear(float zNear) override = 0;

    /**
    *  @brief
    *    Get far plane
    *
    *  @return
    *    Far plane
    */
    virtual float zFar() const override = 0;

    /**
    *  @brief
    *    Set far plane
    *
    *  @param[in] zFar
    *    Far plane
    */
    virtual void setZFar(float zFar) override = 0;

    /**
    *  @brief
    *    Get field-of-view angle (Y)
    *
    *  @return
    *    Angle
    */
    virtual float fovy() const = 0;

    /**
    *  @brief
    *    Set field-of-view angle (Y)
    *
    *  @param[in] fovy
    *    Angle
    */
    virtual void setFovy(float fovy) = 0;

    /**
    *  @brief
    *    Get aspect ratio (width / height)
    *
    *  @return
    *    Aspect ratio
    */
    virtual float aspectRatio() const override = 0;


protected:
    /**
    *  @brief
    *    Set aspect ratio (width / height)
    *
    *  @param[in] ratio
    *    Aspect ratio
    */
    virtual void setAspectRatio(float ratio) = 0;

    /**
    *  @brief
    *    Set aspect ratio by providing a viewport
    *
    *  @param[in] viewport
    *    Viewport size
    */
    virtual void setAspectRatio(const glm::ivec2 & viewport) = 0;

    /**
    *  @brief
    *    Called when the viewport has been changed
    */
    virtual void onViewportChanged();
};


} // namespace gloperate
