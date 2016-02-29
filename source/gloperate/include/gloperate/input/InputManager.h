
#pragma once

#include <list>
#include <memory>

#include <gloperate/input/InputEvent.h>
#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>

#include <gloperate/gloperate_api.h>

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
    InputManager() = default;

    /**
    *  @brief
    *    Destructor
    */
    ~InputManager() = default;

    /**
    *  @brief
    *    Register an eventConsumer to the InputManager
    *  @param[in] consumer
    *    The consumer to register to the InputManager (must NOT be null)
    */
    void registerConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Deregister an eventConsumer from the InputManager
    *  @param[in] consumer
    *    The consumer to deregister from the InputManager (must NOT be null)
    */
    void deregisterConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Add a device to the InputManager
    *  @param[in] device
    *    The device to add to the InputManager (must NOT be null)
    */
    void addDevice(AbstractDevice * device);

    /**
     * @brief onEvent
     *   Forwards the Event to all registered Consumers
     * @param event
     *   The Event to forward
     */
    void onEvent(InputEvent * event);

protected:
    std::list<AbstractEventConsumer *> m_consumers;
    std::list<std::unique_ptr<AbstractDeviceProvider>> m_deviceProviders;
    std::list<std::unique_ptr<AbstractDevice>> m_devices;
    std::list<std::unique_ptr<InputEvent>> m_events;
};


} // namespace gloperate
