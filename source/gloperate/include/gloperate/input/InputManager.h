
#pragma once


#include <list>
#include <memory>

#include <gloperate/input/InputEvent.h>
#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>


namespace gloperate
{


class AbstractEventConsumer;
class AbstractDeviceProvider;
class AbstractDevice;


/**
*  @brief
*    Manager for Input and Devices
*/
class GLOPERATE_API InputManager
{
public:
    /**
    *  @brief
    *    Constructor
    */
    InputManager();

    /**
    *  @brief
    *    Destructor
    */
    ~InputManager();

    /**
    *  @brief
    *    Register an eventConsumer to the InputManager
    *
    *  @param[in] consumer
    *    The consumer to register to the InputManager (must NOT be null)
    */
    void registerConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Deregister an eventConsumer from the InputManager
    *
    *  @param[in] consumer
    *    The consumer to deregister from the InputManager (must NOT be null)
    */
    void deregisterConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Add a device to the InputManager
    *
    *  @param[in] device
    *    The device to add to the InputManager (must NOT be null)
    */
    void addDevice(AbstractDevice * device);

    /**
    *  @brief
    *    Forwards an Event to all registered Consumers
    *
    *  @param event
    *    The Event to forward
    */
    void onEvent(InputEvent * event);


protected:
    std::list<AbstractEventConsumer *>  m_consumers;
    std::list<AbstractDeviceProvider *> m_deviceProviders;
    std::list<AbstractDevice *>         m_devices;
    std::list<InputEvent *>             m_events;
};


} // namespace gloperate
