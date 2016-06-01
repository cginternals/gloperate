
#pragma once


#include <list>
#include <memory>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractEventConsumer;
class AbstractDeviceProvider;
class AbstractDevice;
class InputEvent;


/**
*  @brief
*    Manager for input device and consumers
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
    *    Register an event consumer at the input manager
    *
    *  @param[in] consumer
    *    Event consumer (must NOT be null)
    */
    void registerConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Deregister an event consumer from the input manager
    *
    *  @param[in] consumer
    *    Event consumer (must NOT be null)
    */
    void deregisterConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Add a device to the input manager
    *
    *  @param[in] device
    *    Input device (must NOT be null)
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
