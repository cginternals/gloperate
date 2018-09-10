
#pragma once


#include <gloperate/input/InputEvent.h>


namespace gloperate
{


class GLOPERATE_API MouseEvent : public InputEvent
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    The type of the MouseEvent (must be either of InputEvent::Type::MouseButtonPress,
    *    InputEvent::Type::MouseButtonRelease or InputEvent::Type::MouseMove)
    *  @param[in] dispatchingDevice
    *    Pointer to the device that generated the event (must NOT be null)
    *  @param[in] pos
    *    The current position of the Mouse
    *  @param[in] button
    *    The button that was pressed or released (according to gloperate::MouseButton)
    *  @param[in] modifiers
    *    The modifier mask
    */
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, const glm::ivec2 & pos, int button = 0, int modifiers = 0);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    The type of the AxisEvent (must be InputEvent::Type::MouseWheelScroll)
    *  @param[in] dispatchingDevice
    *    Pointer to the device that generated the event (must NOT be null)
    *  @param[in] pos
    *    The current position of the Mouse
    *  @param[in] wheelDelta
    *    The delta of the mouse wheel
    *  @param[in] modifiers
    *    The modifier mask
    */
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, const glm::ivec2 & pos, const glm::vec2 & wheelDelta, int modifiers = 0);

    /**
    *  @brief
    *    Gets the position of the mouse at this Event
    *
    *  @return
    *    The position of the mouse
    */
    const glm::ivec2 & pos() const;

    /**
    *  @brief
    *    Gets the button involved in this event
    *
    *  @return
    *    The button according to gloperate::MouseButton
    */
    int button() const;

    /**
    *  @brief
    *    Gets the modifier mask active in this event
    *
    *  @return
    *    The modifier mask with bits set according to gloperate::KeyModifier
    */
    int modifiers() const;

    /**
    *  @brief
    *    Gets the wheelDelta of this event
    *
    *  @return
    *    The wheel delta (most mice only have y)
    */
    const glm::vec2 & wheelDelta() const;

    // Virtual InputEvent interface
    virtual std::string asString() const override;


protected:
    glm::ivec2 m_pos;
    int        m_button;
    int        m_modifiers;
    glm::vec2  m_wheelDelta;
};


} // namespace gloperate
