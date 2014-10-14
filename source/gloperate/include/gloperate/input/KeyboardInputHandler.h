#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/input/input.h>


namespace gloperate
{


class GLOPERATE_API KeyboardInputHandler
{


public:
    KeyboardInputHandler();
    virtual ~KeyboardInputHandler();

    virtual void onKeyDown(gloperate::Key key);
    virtual void onKeyUp(gloperate::Key key);


};


} // namespace gloperate
