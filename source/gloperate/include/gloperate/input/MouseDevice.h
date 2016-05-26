
#pragma once


#include <glm/glm.hpp>

#include <gloperate/input/AbstractDevice.h>


namespace gloperate
{


class GLOPERATE_API MouseDevice : public AbstractDevice
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
    MouseDevice(InputManager * InputManager, const std::string & deviceDescriptor);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MouseDevice();

    /**
    *  @brief
    *    Generate a MouseEvent of type MouseMove
    *
    *  @param[in] pos
    *    The new position of the mouse
    */
    void move(const glm::ivec2 & pos);

    /**
    *  @brief
    *    Generate a ButtonEvent of type MouseButtonPress
    *
    *  @param[in] key
    *    The mouse button that was pressed
    *  @param[in] pos
    *    The position of the mouse at this point in time
    */
    void buttonPress(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Generate a ButtonEvent of type MouseButtonRelease
    *
    *  @param[in] key
    *    The mouse button that was released
    *  @param[in] pos
    *    The position of the mouse at this point in time
    */
    void buttonRelease(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Generate a ButtonEvent of type MouseWheelScroll
    *
    *  @param[in] delta
    *    The delta of the mouseWheel (the x-component will be 0 for most mice)
    *  @param[in] pos
    *    The position of the mouse at this point in time
    */
    void wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos);

    // Virtual AbstractDevice interface
    virtual void update() override;
};


} // namespace gloperate
