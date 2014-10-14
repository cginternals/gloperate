#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {


class GLOPERATE_API AbstractViewportCapability : public AbstractCapability
{


public:
    AbstractViewportCapability();
    virtual ~AbstractViewportCapability();
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setViewport(int x, int y, int width, int height) = 0;


};


} // namespace gloperate
