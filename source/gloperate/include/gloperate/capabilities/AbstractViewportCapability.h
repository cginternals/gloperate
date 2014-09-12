#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>

namespace gloperate {

class Viewport;

class GLOPERATE_API AbstractViewportCapability : public AbstractCapability
{
public:
    AbstractViewportCapability();
    virtual ~AbstractViewportCapability();

    virtual void setViewport(const Viewport & viewport) = 0;
    virtual const Viewport & viewport() const = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
};

} // namespace gloperate
