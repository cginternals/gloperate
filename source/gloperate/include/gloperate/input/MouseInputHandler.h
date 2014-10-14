#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/input/input.h>


namespace gloperate
{


class GLOPERATE_API MouseInputHandler
{


public:
    MouseInputHandler();
    virtual ~MouseInputHandler();

    virtual void onMouseMove(int x, int y);
    virtual void onMousePress(int x, int y, gloperate::MouseButton button);
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button);
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button);
    virtual void onMouseWheel(int dx, int dy);


};


} // namespace gloperate
