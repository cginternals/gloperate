#pragma once

#include <signalzeug/Signal.h>

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/ButtonControl.h>
#include <gloperate/input/AxisControl.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>

#include <hidapi/hidapi.h>


namespace gloperate_hidapi
{

class GLOPERATE_HIDAPI_API SpaceNavigator : public gloperate::AbstractDevice
{
public:
    /**
     * @brief 
     *   Constructor
     */
	SpaceNavigator();

    /**
     * @brief 
     *   Destructor
     */
    virtual ~SpaceNavigator();

    /**
     * @brief
     *   Updates the state of the device and the corresponding controls.
     *   This will fire signals from the controls.
     */
    void virtual update();

    gloperate::ButtonControl m_control;
    gloperate::AxisControl m_xAxis;


private:
	hid_device *m_pHandle;
    static const int m_timeout = 10;
};

}
