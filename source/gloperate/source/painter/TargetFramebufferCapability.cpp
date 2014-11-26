#include <gloperate/painter/TargetFramebufferCapability.h>


namespace gloperate
{

/**
*  @brief
*    Constructor
*/
TargetFramebufferCapability::TargetFramebufferCapability()
{
}

/**
*  @brief
*    Destructor
*/
TargetFramebufferCapability::~TargetFramebufferCapability()
{
}

/**
*  @brief
*    Get framebuffer
*/
globjects::Framebuffer * TargetFramebufferCapability::framebuffer() const
{
    return m_framebuffer;
}

/**
*  @brief
*    Set framebuffer
*/
void TargetFramebufferCapability::setFramebuffer(globjects::Framebuffer * fbo)
{
    m_framebuffer = fbo;

    setChanged(true);
}

} // namespace gloperate
