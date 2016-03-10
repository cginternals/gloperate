
#pragma once

#include <gloperate/gloperate_api.h>
#include <string>

namespace gloperate
{

class InputManager;


/**
*  @brief
*    An Interface for devices
*
*    This is a virtual interface for Devices (to be held by an InputManager)
*/
class GLOPERATE_API AbstractDevice
{
public:
    /**
     * @brief
     *   Constructor
     *
     * @param[in] inputManager
     *   A pointer to the inputManager (must NOT be null)
     */
    AbstractDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
     * @brief
     *   Destructor
     */
    virtual ~AbstractDevice();

    /**
     * @brief
     *   Polls the current state of the Device and dispatches appropriate Events
     */
    virtual void update() = 0;

    /**
     * @brief
     *   get the deviceDescriptor as a string
     * @return
     *   the deviceDescriptor as a string
     */
    virtual const std::string& deviceDescriptor() const;

protected:
    InputManager * m_inputManager;
    std::string m_deviceDescriptor;
};

}
