
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractControl.h>

#include <signalzeug/Signal.h>

namespace gloperate
{

class AxisControl : public AbstractControl
{
public:
    /**
     * @brief
     *   Constructor
     */
    GLOPERATE_API AxisControl() = default;

    /**
     * @brief
     *   Destructor
     */
    GLOPERATE_API virtual ~AxisControl() = default;

    /**
     * @brief
     *   Sets the position of the axis and signals listeners.
     */
    GLOPERATE_API void setPosition(float position);


    signalzeug::Signal<float> changed;

private:
    float m_position;
};

}
