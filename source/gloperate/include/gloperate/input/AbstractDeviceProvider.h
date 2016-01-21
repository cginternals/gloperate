
#include <gloperate/gloperate_api.h>

#include <vector>
#include <memory>

namespace gloperate
{

class AbstractDevice;
class InputManager;

class AbstractDeviceProvider
{
public:
    /**
     * @brief
     *   Constructor
     */
    AbstractDeviceProvider() = default;

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

    /**
     *  @brief
     *    Lists all active devices handled by this provider
     *  @return
     *    Returns a vector of all devices connected through this provider
     */
    virtual std::vector<std::shared_ptr<AbstractDevice>> listDevices() = 0;

protected:
    InputManager* m_pinputManager;

};

}
