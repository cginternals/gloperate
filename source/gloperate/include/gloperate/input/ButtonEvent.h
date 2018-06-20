
#pragma once


#include <gloperate/input/InputEvent.h>


namespace gloperate
{


class GLOPERATE_API ButtonEvent : public InputEvent
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    The type of the ButtonEvent (must be either of InputEvent::Type::ButtonPress or InputEvent::Type::ButtonRelease)
    *  @param[in] dispatchingDevice
    *    Pointer to the device that generated the event (must NOT be null)
    *  @param[in] key
    *    The key code
    *  @param[in] modifier
    *    Th active modifiers
    */
    ButtonEvent(Type type, AbstractDevice * dispatchingDevice, int key, int modifier);

    /**
    *  @brief
    *    Get key code
    *
    *  @return
    *    The key code
    */
    int key() const;

    /**
    *  @brief
    *    Get modifier
    *
    *  @return
    *    The modifier
    */
    int modifier() const;


    // Virtual InputEvent interface
    virtual std::string asString() const override;

protected:
    int m_key;
    int m_modifier;
};


} // namespace gloperate
