
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
    *  @param[in] description
    *    A string decribing the pressed button
    */
    ButtonEvent(Type type, AbstractDevice * dispatchingDevice, const std::string & description);

    /**
    *  @brief
    *    A getter for the description of the event
    *
    *  @return
    *    A string decribing the pressed button
    */
    std::string description() const;

    // Virtual InputEvent interface
    virtual std::string asString() const override;


protected:
    std::string m_description;
};

} // namespace gloperate
