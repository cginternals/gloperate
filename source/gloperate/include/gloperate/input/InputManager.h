
#pragma once


#include <list>
#include <vector>
#include <memory>

#include <gloperate/gloperate_api.h>
#include <gloperate/input/Mapping.h>


namespace gloperate
{


class AbstractEventConsumer;
class AbstractDeviceProvider;
class AbstractDevice;
class InputEvent;
class Control;


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
    *    Add a mapping to the input manager
    *
    *  @param[in] mapping
    *    mapping (must NOT be null)
    */
    void addMapping(Mapping * mapping);

    /**
    *  @brief
    *    Forwards an Event to all registered Consumers
    *
    *  @param[in] event
    *    The Event to forward
    */
    void onEvent(InputEvent * event);

    /**
    *  @brief
    *    Forwards an Event to all listening metaphors
    *
    * @param[in] control
    *    The dispatching control
    *
    *  @param[in] event
    *    The Event to forward
    */
    void onControlEvent(Control * control, InputEvent * event);


protected:
    std::list<AbstractEventConsumer *>  m_consumers;
    std::list<AbstractDeviceProvider *> m_deviceProviders;
    std::list<AbstractDevice *>         m_devices;
    std::list<InputEvent *>             m_events;
    std::vector<Mapping *>              m_mappings;
};


} // namespace gloperate
