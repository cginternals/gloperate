
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractCapability.h>


namespace gloperate
{


class AbstractViewportCapability;
class Camera;


/**
*  @brief
*    Capability that allows for accessing a painter camera
*/
class GLOPERATE_API AbstractCameraCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractCameraCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCameraCapability();

    /**
    *  @brief
    *    Resets the camera to a subclass-defined specific state
    */
    virtual void reset() = 0;

    /**
    *  @brief
    *    Get camera (eye) position
    *
    *  @return
    *    Camera position
    */
    virtual const glm::vec3 & eye() const = 0;

    /**
    *  @brief
    *    Set camera (eye) position
    *
    *  @param[in] eye
    *    Camera position
    */
    virtual void setEye(const glm::vec3 & eye) = 0;

    /**
    *  @brief
    *    Get look-at (center) position
    *
    *  @return
    *    Look-at position
    */
    virtual const glm::vec3 & center() const = 0;

    /**
    *  @brief
    *    Set look-at (center) position
    *
    *  @param[in] center
    *    Look-at position
    */
    virtual void setCenter(const glm::vec3 & center) = 0;

    /**
    *  @brief
    *    Get up-vector
    *
    *  @return
    *    Up-vector
    */
    virtual const glm::vec3 & up() const = 0;

    /**
    *  @brief
    *    Set up-vector
    *
    *  @param[in] up
    *    Up-vector
    */
    virtual void setUp(const glm::vec3 & up) = 0;

    /**
    *  @brief
    *    Get view matrix
    *
    *  @return
    *    View matrix
    */
    virtual const glm::mat4 & view() const = 0;

    /**
    *  @brief
    *    Get inverted view matrix
    *
    *  @return
    *    Inverted view matrix
    */
    virtual const glm::mat4 & viewInverted() const = 0;

    /**
    *  @brief
    *    Get normal matrix
    *
    *    [TODO] What is this exactly?
    *
    *  @return
    *    Inverted view matrix
    */
    virtual const glm::mat3 & normal() const = 0;
};


} // namespace gloperate
