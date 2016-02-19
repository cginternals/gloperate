#pragma once

namespace gloperate
{

class InputManager;

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
    AbstractDevice(InputManager * inputManager);

    /**
     * @brief
     *   Destructor
     */
    virtual ~AbstractDevice();

    /**
     * @brief
     *   Poll the current state of the Device and fire appropriate Signals
     */
    virtual void update() = 0;

private:
    InputManager * m_inputManager;
};

}
