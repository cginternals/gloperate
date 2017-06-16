
#pragma once


#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

#include <gloperate/rendering/RenderTargetType.h>
#include <gloperate/rendering/AttachmentType.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Framebuffer;
    class FramebufferAttachment;
    class Renderbuffer;
    class Texture;
}


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
    *    Get attachment type
    *
    *  @return
    *    The attachment type
    */
    AttachmentType attachmentType() const;

    /**
    *  @brief
    *    Get attachment type as GLenum
    *
    *  @return
    *    The attachment type as GLenum
    */
    gl::GLenum attachmentGLType() const;

    /**
    *  @brief
    *    Set attachment type
    *
    *  @param[in] type
    *    The attachment type
    */
    void setAttachmentType(AttachmentType attachmentType);

    /**
    *  @brief
    *    Set desired target
    *
    *  @param[in] texture
    *    A texture
    */
    void setTarget(globjects::Texture * texture);

    /**
    *  @brief
    *    Set desired target
    *
    *  @param[in] renderbuffer
    *    A renderbuffer
    */
    void setTarget(globjects::Renderbuffer * renderbuffer);

    /**
    *  @brief
    *    Set desired target
    *
    *  @param[in] attachment
    *    A symbolic attachment of the default renderbuffer
    */
    void setTarget(gl::GLenum attachment);

    /**
    *  @brief
    *    Set desired target
    *
    *  @param[in] fboAttachment
    *    A user-defined fbo attachment
    */
    void setTarget(globjects::FramebufferAttachment * fboAttachment);

    /**
    *  @brief
    *    Get current target type
    *
    *  @return
    *    The current target type
    */
    RenderTargetType type() const;

    /**
    *  @brief
    *    Get default framebuffer attachment
    *
    *  @return
    *    The default framebuffer attachment
    *
    *  @remarks
    *    The result is only defined if type() == DefaultFBOAttachment
    */
    gl::GLenum defaultFramebufferAttachment() const;

    /**
    *  @brief
    *    Get texture attachment
    *
    *  @return
    *    The texture attachment
    *
    *  @remarks
    *    The result is only defined if type() == Texture
    */
    globjects::Texture * textureAttachment() const;

    /**
    *  @brief
    *    Get renderbuffer attachment
    *
    *  @return
    *    The renderbuffer attachment
    *
    *  @remarks
    *    The result is only defined if type() == Renderbuffer
    */
    globjects::Renderbuffer * renderbufferAttachment() const;

    /**
    *  @brief
    *    Get framebuffer attachment
    *
    *  @return
    *    The framebuffer attachment
    *
    *  @remarks
    *    The result is only defined if type() == UserDefinedFBOAttachment
    */
    globjects::FramebufferAttachment * framebufferAttachment() const;

    /**
    *  @brief
    *    Query requirement for user-defined framebuffer to use this attachment
    *
    *  @return
    *    'true', if a user-defined framebuffer is required to use this attachment
    *
    *  @remarks
    *    A user-defined framebuffer is required if the attachment is neither invalid
    *    nor an attachment of the default framebuffer
    */
    bool attachmentRequiresUserDefinedFramebuffer() const;

    gl::GLenum attachmentBuffer() const;

    gl::GLint attachmentDrawBuffer(gl::GLint index) const;

protected:
    RenderTargetType                   m_type;           ///< Target type
    AttachmentType                     m_attachmentType; ///< OpenGL attachment type
    gl::GLenum                         m_attachment;     ///< Default framebuffer attachment target
    globjects::Texture               * m_texture;        ///< Texture target
    globjects::Renderbuffer          * m_renderbuffer;   ///< Renderbuffer target
    globjects::FramebufferAttachment * m_userDefined;    ///< User defined framebuffer attachment target
};


} // namespace gloperate
