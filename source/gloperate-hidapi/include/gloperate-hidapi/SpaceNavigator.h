#pragma once

#include <vector>

#include <signalzeug/Signal.h>

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/ButtonControl.h>
#include <gloperate/input/AxisControl.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>

#include <hidapi/hidapi.h>

using gloperate::AxisControl;


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
    AxisControl m_xTranslation;
    AxisControl m_yTranslation;
    AxisControl m_zTranslation;
    AxisControl m_xRotation;
    AxisControl m_yRotation;
    AxisControl m_zRotation;

    //std::vector<AxisControl&> m_Axes;


private:
	hid_device *m_pHandle;
    static const int m_timeout = 10;
};

}
