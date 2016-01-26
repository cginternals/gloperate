
#include <gloperate/painter/TargetFramebufferCapability.h>


namespace gloperate
{


TargetFramebufferCapability::TargetFramebufferCapability()
{
}

TargetFramebufferCapability::~TargetFramebufferCapability()
{
}

globjects::Framebuffer * TargetFramebufferCapability::framebuffer() const
{
    return m_framebuffer;
}

void TargetFramebufferCapability::setFramebuffer(globjects::Framebuffer * fbo)
{
    m_framebuffer = fbo;

    setChanged(true);
}


} // namespace gloperate
