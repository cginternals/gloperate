
#include <gloperate/gloperate_api.h>

namespace gloperate
{

class InputManager;

class AbstractDeviceProvider
{
public:
    /**
     * @brief
     *   Constructor
     */
    AbstractDeviceProvider(InputManager * manager) 
        : m_pinputManager(manager){};

    /**
     * @brief
     *   Destructor
     */
    virtual ~AbstractDeviceProvider() = default;

    /**
     *  @brief
     *    Updates the device list and adds Devices to the Input Manager
     */
    virtual void updateDevices() = 0;

protected:
    InputManager* m_pinputManager;

};

}
