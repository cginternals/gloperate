
#include <gloperate/navigation/FlyNavigation.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <iostream>

namespace gloperate
{
FlyNavigation::FlyNavigation(
        AbstractCameraCapability& cameraCapability) :
    m_cameraCapability(cameraCapability)
{
}

FlyNavigation::~FlyNavigation()
{
}

void FlyNavigation::move(glm::vec3 delta)
{
    std::cout << delta.length() << std::endl;
    if((-10 < glm::length(delta) && glm::length(delta) < 10))
        return;

    std::cout << "moving" << std::endl;
        
    m_cameraCapability.setEye(m_cameraCapability.eye() + delta/500.f);
    m_cameraCapability.setCenter(m_cameraCapability.center() + delta/500.f);
}

void FlyNavigation::moveX(float dX)
{
    move(glm::vec3{dX,0.0f,0.0f});
}
void FlyNavigation::moveY(float dY)
{
    move(glm::vec3{0.0f,dY,0.0f});
}
void FlyNavigation::moveZ(float dZ)
{
    move(glm::vec3{0.0f,0.0f,dZ});
}

}
