#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>

namespace gloperate {

class GLOPERATE_API VirtualTimeCapability : public AbstractCapability
{
public:
    virtual void update(float delta) = 0;
};

} // namespace gloperate
