
#pragma once


#include <memory>
#include <list>
#include <map>

#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;
class AbstractEventConsumer;
class AbstractDeviceProvider;
class AbstractDevice;
class InputEvent;


/**
*  @brief
*    Manager for input device and consumers
*/
class GLOPERATE_API InputManager : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the manager belongs (must NOT be null!)
    */
    InputManager(Environment * environment);

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
    void onEvent(std::unique_ptr<InputEvent> && event);


protected:
    // Scripting functions
    int  scr_onInput(const cppexpose::Variant & func);


protected:
    Environment                                      * m_environment; ///< Gloperate environment to which the manager belongs
    std::list<AbstractEventConsumer *>                 m_consumers;
    std::list<std::unique_ptr<AbstractDeviceProvider>> m_deviceProviders;
    std::list<AbstractDevice *>                        m_devices;
    std::list<std::unique_ptr<InputEvent>>             m_events;
    std::map<int, cppexpose::Function>                 m_callbacks;
    int                                                m_nextId;      ///< Next callback ID
};


} // namespace gloperate
