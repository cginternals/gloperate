#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>

namespace globjects {

class Framebuffer;

} // namespace globjects

namespace gloperate {

class GLOPERATE_API AbstractTargetFramebufferCapability : public AbstractCapability
{
public:
    AbstractTargetFramebufferCapability();
    virtual ~AbstractTargetFramebufferCapability();

    virtual void setFramebuffer(globjects::Framebuffer * fbo) = 0;
    virtual globjects::Framebuffer * framebuffer() const = 0;
};

} // namespace gloperate
