
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/Framebuffer.h>

#include <gloperate/painter/AbstractTargetFramebufferCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractTargetFramebufferCapability
*/
class GLOPERATE_API TargetFramebufferCapability : public AbstractTargetFramebufferCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    TargetFramebufferCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TargetFramebufferCapability();

    // Virtual functions from AbstractTargetFramebufferCapability
    virtual globjects::Framebuffer * framebuffer() const override;
    virtual void setFramebuffer(globjects::Framebuffer * fbo) override;


protected:
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;    /**< Framebuffer used for rendering */
};


} // namespace gloperate
