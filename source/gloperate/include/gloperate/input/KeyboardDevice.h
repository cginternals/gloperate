
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
    *  @param[in] deviceDescriptor
    *    [TODO] What is a device descriptor?
    */
    KeyboardDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~KeyboardDevice();

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

    // Virtual AbstractDevice interface
    virtual void update() override;
};

} // namespace gloperate
