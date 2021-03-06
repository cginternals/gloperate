
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;


/**
*  @brief
*    Base class for input devices
*
*    This class represents an actual (physical or virtual) input device,
*    e.g., a keyboard, mouse, or tracking device. It is responsible for
*    updating the status of the input device it represents and generating
*    the appropriate events for them.
*
*    Input devices are owned by the input manager and usually managed
*    (inserted and removed) by a device provider.
*/
class GLOPERATE_API AbstractDevice
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    Input manager that owns the input device (must NOT be null)
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
    *    Poll the current state of the device and dispatch appropriate events
    */
    virtual void update() = 0;

    /**
    *  @brief
    *    Get device descriptor
    *
    *  @return
    *    [TODO] What is a device descriptor
    */
    virtual const std::string & deviceDescriptor() const;


protected:
    InputManager * m_inputManager;     ///< Input manager that owns the device (must NOT be null!)
    std::string    m_deviceDescriptor; ///< [TODO] What is a device descriptor
};


} // namespace gloperate
