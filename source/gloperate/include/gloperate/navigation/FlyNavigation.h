

#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/navigation/AbstractInteraction.h>



namespace gloperate
{


class AbstractCameraCapability;

/**
*  @brief
*    Camera navigation 'flying'
*
*    This interaction technique provides a 'flying' metaphor
*    to control a virtual camera. It allows the user to move and rotate
*    the camera freely in the world
*/


class GLOPERATE_API FlyNavigation : public AbstractInteraction
{
public:
    FlyNavigation(AbstractCameraCapability & cameraCapability);
    virtual ~FlyNavigation();

    void move(glm::vec3 delta);
    void rotate(glm::quat delta);

public:
    //Slots for the control signals
    void moveX(float dX);
    void moveY(float dY);
    void moveZ(float dZ);

//    void rotateX(float dX);
//    void rotateY(float dY);
//    void rotateZ(float dZ);

protected:
    AbstractCameraCapability & m_cameraCapability;

};


} // namespace gloperate
