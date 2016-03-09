
#pragma once

#include <gloperate/input/InputEvent.h>

namespace gloperate{

class GLOPERATE_API MouseEvent : public InputEvent
{
public:

    /**
     * @brief
     *   Constructor
     *
     * @param[in] type
     *   The type of the AxisEvent (must be either of InputEvent::Type::MouseButtonPress,
     *   InputEvent::Type::MouseButtonRelease or InputEvent::Type::MouseMove)
     *
     * @param[in] dispatchingDevice
     *   Pointer to the device that generated the event (must NOT be null)
     *
     * @param pos
     *   The current position of the Mouse
     *
     * @param button
     *   The button that was pressed or released (according to gloperate::MouseButton)
     */
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, int button = 0);

    /**
     * @brief
     *   Constructor
     *
     * @param[in] type
     *   The type of the AxisEvent (must be InputEvent::Type::MouseWheelScroll)
     *
     * @param[in] dispatchingDevice
     *   Pointer to the device that generated the event (must NOT be null)
     *
     * @param pos
     *   The current position of the Mouse
     *
     * @param wheelDelta
     *   The delta of the mouse wheel
     */
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::vec2 wheelDelta);

    /**
     * @brief
     *   Gets the position of the mouse at this Event
     * @return
     *   The position of the mouse
     */
    glm::ivec2 pos() const;

    /**
     * @brief
     *   Gets the button involved in this event
     * @return
     *   The button according to gloperate::MouseButton
     */
    int button() const;

    /**
     * @brief
     *   Gets the wheelDelta of this event
     * @return
     *   The wheel delta (most mice only have y)
     */
    glm::vec2 wheelDelta() const;


    /**
     * @brief asString
     *   A getter for the description of the Event as a string
     * @return
     *   A string describing the event
     */
    virtual std::string asString() const override;

protected:
    glm::ivec2 m_pos;
    int m_button;
    glm::vec2 m_wheelDelta;
};

} //namespace gloperate
