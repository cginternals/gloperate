
#pragma once


#include <gloperate/input/InputEvent.h>


namespace gloperate
{


class GLOPERATE_API AxisEvent : public InputEvent
{
public:
    /**
     * @brief
     *   Constructor
     *
     * @param[in] type
     *   The type of the AxisEvent (must be InputEvent::Type::SpatialAxis)
     *
     * @param[in] dispatchingDevice
     *   Pointer to the device that generated the event (must NOT be null)
     *
     * @param[in] value
     *   The 3x3 matrix to be held by the event
     */
    AxisEvent(Type type, AbstractDevice * dispatchingDevice, glm::mat3 value);

    /**
     * @brief
     *   A getter for the value of the event
     *
     * @return
     *   The value held by the event
     */
    const glm::mat3& value() const;

    /**
     * @brief asString
     *   A getter for the description of the Event as a string
     * @return
     *   A string describing the event
     */
    virtual std::string asString() const override;

protected:
    glm::mat3 m_value;
};

} //namespace gloperate
