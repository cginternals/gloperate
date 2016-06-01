
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;


/**
*  @brief
*    Base class for input device providers
*
*    A device provider is responsible for managing and updating input devices
*    of a certain kind. For example, a HID input device provider might manage
*    all HID devices is detects on the computer.
*
*    A device provider registers and unregisters devices at the input manager
*    when it detects them and updates the devices (i.e., polling events).
*/
class GLOPERATE_API AbstractDeviceProvider
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractDeviceProvider();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDeviceProvider();

    /**
    *  @brief
    *    Update device list, add and remove devices on the input manager
    */
    virtual void updateDevices() = 0;


protected:
    InputManager * m_inputManager; ///< Input manager that owns the device provider
};

} // namespace gloperate
