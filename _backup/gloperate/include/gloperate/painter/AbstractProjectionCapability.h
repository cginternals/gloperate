
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractCapability.h>


namespace gloperate
{


class AbstractViewportCapability;


/**
*  @brief
*    Base class for camera projection capabilities
*/
class GLOPERATE_API AbstractProjectionCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewportCapability
    *    Viewport capability (must NOT be null!)
    */
    AbstractProjectionCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractProjectionCapability();

    /**
    *  @brief
    *    Get near plane
    *
    *  @return
    *    Near plane
    */
    virtual float zNear() const = 0;

    /**
    *  @brief
    *    Set near plane
    *
    *  @param[in] zNear
    *    Near plane
    */
    virtual void setZNear(float zNear) = 0;

    /**
    *  @brief
    *    Get far plane
    *
    *  @return
    *    Far plane
    */
    virtual float zFar() const = 0;

    /**
    *  @brief
    *    Set far plane
    *
    *  @param[in] zFar
    *    Far plane
    */
    virtual void setZFar(float zFar) = 0;

    /**
    *  @brief
    *    Get aspect ratio (width / height)
    *
    *  @return
    *    Aspect ratio
    */
    virtual float aspectRatio() const = 0;

    /**
    *  @brief
    *    Get projection matrix
    *
    *  @return
    *    Projection matrix
    */
    virtual const glm::mat4 & projection() const = 0;

    /**
    *  @brief
    *    Get inverted projection matrix
    *
    *  @return
    *    Inverted projection matrix
    */
    virtual const glm::mat4 & projectionInverted() const = 0;

    /**
    *  @brief
    *    Get projection matrix for a specified aspect ratio
    *
    *  @param[in] ratio
    *    Aspect ratio
    *
    *  @return
    *    Projection matrix
    */
    virtual glm::mat4 projectionForAspectRatio(float ratio) const = 0;


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


protected:
    AbstractViewportCapability * m_viewportCapability;  /**< Viewport capability (must NOT be null!) */
};


} // namespace gloperate
