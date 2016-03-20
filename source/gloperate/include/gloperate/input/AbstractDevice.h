
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;


/**
*  @brief
*    An Interface for devices
*
*    This is a virtual interface for devices (to be held by an InputManager)
*/
class GLOPERATE_API AbstractDevice
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    A pointer to the input manager (must NOT be null)
    *  @param[in] deviceDescriptor
    *    [TODO] What is a device descriptor?
    */
    AbstractDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDevice();

    /**
    *  @brief
    *    Poll the current state of the device and dispatche appropriate events
    */
    virtual void update() = 0;

    /**
    *  @brief
    *    Get device descriptor as a string
    *
    *  @return
    *    [TODO] What is a device descriptor
    */
    virtual const std::string & deviceDescriptor() const;


protected:
    InputManager * m_inputManager;
    std::string    m_deviceDescriptor;
};


} // namespace gloperate
