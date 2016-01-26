
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/Framebuffer.h>

#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractTypedRenderTargetCapability
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

    // Virtual functions from AbstractTypedRenderTargetCapability
    virtual bool hasRenderTarget(RenderTargetType type) override;
    virtual const RenderTarget & renderTarget(RenderTargetType type) override;

    /**
    *  @brief
    *    Set a render target
    *
    *  @param[in] type
    *    Data type
    *  @param[in] framebuffer
    *    Frame buffer object
    *  @param[in] attachment
    *    Attachment in FBO
    *  @param[in] format
    *    Data format
    */
    virtual void setRenderTarget(
        RenderTargetType type, 
        globjects::ref_ptr<globjects::Framebuffer> framebuffer,
        gl::GLenum attachment,
        gl::GLenum format);

    /**
    *  @brief
    *    Reset render target to empty data
    *
    *  @param[in] type
    *    Data type
    */
    virtual void resetRenderTarget(RenderTargetType type);


protected:
    std::map<RenderTargetType, RenderTarget> m_renderTargets;   /**< Render target data */
};


} // namespace gloperate
