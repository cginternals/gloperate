#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractTargetFramebufferCapability.h>

#include <globjects/Framebuffer.h>
#include <globjects/base/ref_ptr.h>

namespace gloperate
{

class GLOPERATE_API TargetFramebufferCapability : public AbstractTargetFramebufferCapability
{
public:
    TargetFramebufferCapability();
    virtual ~TargetFramebufferCapability();

    virtual void setFramebuffer(globjects::Framebuffer * fbo) override;
    virtual globjects::Framebuffer * framebuffer() const override;

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;
};

} // namespace gloperate
