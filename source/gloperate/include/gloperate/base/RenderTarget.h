
#pragma once


#include <globjects/Framebuffer.h>
#include <globjects/base/ref_ptr.h>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Pointer to a render target
*
*    Points to a render target given by a framebuffer and an attachment
*    within that framebuffer. It also contains the format of the render target.
*/
class GLOPERATE_API RenderTarget
{
public:
    /**
    *  @brief
    *    Constructor
    */
    RenderTarget();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] framebuffer
    *    Framebuffer containing the render target (texture)
    *  @param[in] attachment
    *    Attachment within the given framebuffer
    *  @param[in] format
    *    Format of the render target
    */
    RenderTarget(
        globjects::ref_ptr<globjects::Framebuffer> framebuffer,
        gl::GLenum attachment,
        gl::GLenum format);

    /**
    *  @brief
    *    Copy Constructor
    *
    *  @param[in] renderTarget
    *    Source RenderTarget
    */
    RenderTarget(const RenderTarget & renderTarget);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderTarget();

    /**
    *  @brief
    *    Check if object points to a valid render target
    *
    *  @return
    *    'true' if the render target is valid, else 'false'
    */
    bool isValid() const;

    /**
    *  @brief
    *    Get framebuffer
    *
    *  @return
    *    Framebuffer
    */
    globjects::ref_ptr<globjects::Framebuffer> framebuffer() const;

    /**
    *  @brief
    *    Get attachment
    *
    *  @return
    *    Framebuffer attachment
    */
    gl::GLenum attachment() const;

    /**
    *  @brief
    *    Get format of attachment
    *
    *  @return
    *    Attachment format
    */
    gl::GLenum format() const;


protected:
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;   ///< Pointer to framebuffer
    gl::GLenum                                 m_attachment;    ///< Attachment ID
    gl::GLenum                                 m_format;        ///< Format of the attachment
    bool                                       m_valid;         ///< 'true' if render target is valid, else 'false'
};


} // namespace gloperate
