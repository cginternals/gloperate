#pragma once

#include <gloperate-hid/gloperate-hidapi_api.h>

namespace gloperate_hidapi
{

class GLOPERTATE_HIDAPI_API SpaceNavigator
{
public:
	SpaceNavigator();
	void checkEvents(float timeout);
};
