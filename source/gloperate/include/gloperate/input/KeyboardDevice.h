
#pragma once

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API KeyboardDevice : public AbstractDevice
{
public:
    /**
     * @brief
     *   Constructor
     *
     * @param[in] inputManager
     *   A pointer to the inputManager (must NOT be null)
     */
    KeyboardDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
     * @brief
     *   Destructor
     */
    virtual ~KeyboardDevice();

    /**
     * @brief
     *   Poll the current state of the Device and fire appropriate Signals
     */
    virtual void update() override;

    void keyPress(int key, int modifier);
    void keyRelease(int key, int modifier);
};

}
