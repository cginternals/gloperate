/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/TargetFramebufferCapability.h>


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
}



} // namespace gloperate
