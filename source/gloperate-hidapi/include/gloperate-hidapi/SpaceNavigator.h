#pragma once

#include <gloperate-hidapi/gloperate-hidapi_api.h>
#include <hidapi/hidapi.h>


namespace gloperate_hidapi
{

class GLOPERATE_HIDAPI_API SpaceNavigator
{
public:
	SpaceNavigator();
	~SpaceNavigator();
	void checkEvents(int timeout);

private:
	hid_device *m_pHandle;
};

}
