#include <gloperate/capabilities/TargetFramebufferCapability.h>

namespace gloperate
{

TargetFramebufferCapability::TargetFramebufferCapability()
{
}

TargetFramebufferCapability::~TargetFramebufferCapability()
{
}

void TargetFramebufferCapability::setFramebuffer(globjects::Framebuffer * fbo)
{
    m_framebuffer = fbo;
}

globjects::Framebuffer * TargetFramebufferCapability::framebuffer() const
{
    return m_framebuffer;
}

} // namespace gloperate
