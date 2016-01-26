
#pragma once


#include <gloperate/painter/AbstractCapability.h>


namespace globjects 
{
    class Framebuffer;
}


namespace gloperate 
{


/**
*  @brief
*    Capability that allows the target framebuffer to be set for rendering
*
*    If a painter supports this capability, it can be supplied with a framebuffer
*    object that it will use for rendering into.
*/
class GLOPERATE_API AbstractTargetFramebufferCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractTargetFramebufferCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractTargetFramebufferCapability();

    /**
    *  @brief
    *    Get framebuffer
    *
    *  @return
    *    Framebuffer, can be nullptr
    */
    virtual globjects::Framebuffer * framebuffer() const = 0;

    /**
    *  @brief
    *    Set framebuffer
    *
    *  @param[in] fbo
    *    Framebuffer, can be nullptr
    */
    virtual void setFramebuffer(globjects::Framebuffer * fbo) = 0;
};


} // namespace gloperate
