
#pragma once


#include <glbinding/gl/enum.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/rendering/RenderTargetType.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{


class Framebuffer;
class FramebufferAttachment;
class Renderbuffer;
class Texture;


} // namespace globjects


namespace gloperate
{


/**
*  @brief
*    Render target that represents one target we can render into
*
*    A render target can internally be: a texture, a renderbuffer,
*    a symbolic attachment of the default renderbuffer, or a user-defined
*    renderbuffer with attachment specification.
*/
class GLOPERATE_API RenderTarget : public globjects::Referenced
{
public:
    /**
    *  @brief
    *    Constructor
    */
    RenderTarget();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderTarget();

    /**
    *  @brief
    *    Release the current target
    */
    void releaseTarget();

    /**
    *  @brief
    *    Set a (new) desired target
    *
    *  @param[in] texture
    *    A texture
    */
    void setTarget(globjects::Texture * texture);

    /**
    *  @brief
    *    Set a (new) desired target
    *
    *  @param[in] renderbuffer
    *    A renderbuffer
    */
    void setTarget(globjects::Renderbuffer * renderbuffer);

    /**
    *  @brief
    *    Set a (new) desired target
    *
    *  @param[in] attachment
    *    A symbolic attachment of the default renderbuffer
    */
    void setTarget(gl::GLenum attachment);

    /**
    *  @brief
    *    Set a (new) desired target
    *
    *  @param[in] fboAttachment
    *    A user-defined fbo attachment
    */
    void setTarget(globjects::FramebufferAttachment *fboAttachment);

    /**
    *  @brief
    *    Binds this render target to a framebuffer
    *
    *  @param[in] renderbuffer
    *    Target framebuffer
    *
    *  @param[in] bindingPoint
    *    Target binding point, e.g. gl::GL_DEPTH_ATTACHMENT
    *    Will be ignored if not applicable
    */
    void bind(gl::GLenum bindingPoint, globjects::Framebuffer * fbo);

    /**
    *  @brief
    *    Unbinds this render target
    */
    // void unbind();



protected:
    RenderTargetType                                         m_type;         ///< the current type

    union {
        gl::GLenum                                           m_attachment;   ///< the default framebuffer attachment target
        globjects::ref_ptr<globjects::Texture>               m_texture;      ///< the texture target
        globjects::ref_ptr<globjects::Renderbuffer>          m_renderbuffer; ///< the renderbuffer target
        globjects::ref_ptr<globjects::FramebufferAttachment> m_userDefined;  ///< the user defined framebuffer attachment
    };

};


} // namespace gloperate
