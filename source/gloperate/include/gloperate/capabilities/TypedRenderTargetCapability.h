/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>
#include <globjects/Framebuffer.h>
#include <globjects/base/ref_ptr.h>
#include <gloperate/capabilities/AbstractTypedRenderTargetCapability.h>


namespace gloperate {


/**
*  @brief
*    
*/
class GLOPERATE_API TypedRenderTargetCapability : public AbstractTypedRenderTargetCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    TypedRenderTargetCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TypedRenderTargetCapability();

    virtual const RenderTarget & renderTarget(RenderTargetType type);
    virtual bool hasRenderTarget(RenderTargetType type);
    virtual void setRenderTarget(
        RenderTargetType type, 
        globjects::ref_ptr<globjects::Framebuffer> framebuffer,
        gl::GLenum attachment,
        gl::GLenum format);

protected:
    std::map<RenderTargetType, RenderTarget *> m_renderTargets;
    RenderTarget m_invalidTarget;
};


} // namespace gloperate
