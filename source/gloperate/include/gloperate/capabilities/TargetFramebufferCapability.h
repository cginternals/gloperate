/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <globjects/Framebuffer.h>
#include <globjects/base/ref_ptr.h>
#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractTargetFramebufferCapability.h>


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

    /**
    *  @brief
    *    Get framebuffer
    *
    *  @return
    *    Framebuffer, can be nullptr
    */
    virtual globjects::Framebuffer * framebuffer() const override;

    /**
    *  @brief
    *    Set framebuffer
    *
    *  @param[in] fbo
    *    Framebuffer, can be nullptr
    */
    virtual void setFramebuffer(globjects::Framebuffer * fbo) override;


protected:
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;	/**< Framebuffer used for rendering */


};


} // namespace gloperate
