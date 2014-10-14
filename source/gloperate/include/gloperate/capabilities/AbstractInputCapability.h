#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/input/input.h>
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {


class GLOPERATE_API AbstractInputCapability : public AbstractCapability
{


public:
    AbstractInputCapability();
    virtual ~AbstractInputCapability();

    virtual void onMouseMove(int x, int y) = 0;
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) = 0;
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) = 0;
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) = 0;
    virtual void onMouseWheel(int dx, int dy) = 0;
    virtual void onKeyDown(gloperate::Key key) = 0;
    virtual void onKeyUp(gloperate::Key key) = 0;


};


} // namespace gloperate
