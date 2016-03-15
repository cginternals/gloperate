
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
*    This is a virtual interface for Devices (to be held by an InputManager)
*/
class GLOPERATE_API AbstractDevice
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    */

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    A pointer to the inputManager (must NOT be null)
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
    *    Polls the current state of the Device and dispatches appropriate Events
    */
    virtual void update() = 0;

    /**
    *  @brief
    *    Get the deviceDescriptor as a string
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
