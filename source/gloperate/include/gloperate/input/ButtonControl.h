
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractControl.h>

#include <signalzeug/Signal.h>

namespace gloperate
{

class ButtonControl : public AbstractControl
{
public:
    /**
     * @brief
     *   Constructor
     */
    GLOPERATE_API ButtonControl() = default;

    /**
     * @brief
     *   Destructor
     */
    GLOPERATE_API virtual ~ButtonControl() = default;

    /**
     * @brief
     *   Sets the button state
     *
     * @param[in] state
     *   The state to set the button to
     */
    GLOPERATE_API void setPressed(bool state);

    /**
     * @brief
     *   Gets the button state
     *
     * @return
     *   Tells whether the button is currently pressed
     */
    GLOPERATE_API bool getPressed();

    signalzeug::Signal<> pressed;
    signalzeug::Signal<> released;

private:
    bool m_state;
};

}
