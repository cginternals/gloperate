#pragma once

#include <signalzeug/Signal.h>

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/ButtonControl.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>

#include <hidapi/hidapi.h>


namespace gloperate_hidapi
{

class GLOPERATE_HIDAPI_API SpaceNavigator : public gloperate::AbstractDevice
{
public:
	SpaceNavigator();
    virtual ~SpaceNavigator();
    void checkEvents(int timeout);
    void virtual update();

    gloperate::ButtonControl m_control;


private:
	hid_device *m_pHandle;
};

}
