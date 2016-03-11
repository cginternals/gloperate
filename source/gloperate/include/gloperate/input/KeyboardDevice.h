
#pragma once


#include <gloperate/input/AbstractDevice.h>


namespace gloperate
{


class GLOPERATE_API KeyboardDevice : public AbstractDevice
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    A pointer to the inputManager (must NOT be null)
    */
    KeyboardDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~KeyboardDevice();

    /**
    *  @brief
    *    Poll the current state of the Device and fire appropriate Signals
    */
    virtual void update() override;

    /**
    *  @brief
    *    Generate a ButtonEvent of type ButtonPress
    *
    *  @param[in] key
    *    The key that was pressed
    *
    *  @param[in] modifier
    *    The modifiers that apply to the key
    */
    void keyPress(int key, int modifier);

    /**
    *  @brief
    *    Generate a ButtonEvent of type ButtonRelease
    *
    *  @param[in] key
    *    The key that was released
    *  @param[in] modifier
    *    The modifiers that apply to the key
    */
    void keyRelease(int key, int modifier);
};


} // namespace gloperate
