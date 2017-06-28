
#pragma once

#include <gloperate/rendering/RenderTargetType.h>
#include <gloperate/rendering/AttachmentType.h>

#include <gloperate/gloperate_api.h>


namespace gl
{
    enum class GLenum : unsigned int;
    using GLint = int;
}


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
*    Abstract render target that represents one target we can render into
*
*    A render target can internally be: a texture, a renderbuffer,
*    a symbolic attachment of the default renderbuffer, or a user-defined
*    renderbuffer with attachment specification.
*/
class GLOPERATE_API AbstractRenderTarget
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractRenderTarget();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractRenderTarget();

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
    AttachmentType underlyingAttachmentType() const;

    /**
    *  @brief
    *    Set attachment type
    *
    *  @param[in] type
    *    The attachment type
    */
    void setUnderlyingAttachmentType(AttachmentType underlyingAttachmentType);

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
    RenderTargetType currentTargetType() const;

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

    /**
    *  @brief
    *    Get the symbolic constant of the attachment type used for glClearBuffer
    *
    *  @return
    *    The symbolic constant for glClearBuffer parameter 1
    */
    gl::GLenum clearBufferAttachment() const;

    /**
    *  @brief
    *    Get the draw buffer attachment index
    *
    *  @param[in] index
    *    The current color attachment index.
    *
    *  @return
    *    The draw buffer attachment index used for glClearBuffer parameter 1
    *
    *  @remarks
    *    If this is no color attachment, '0' is returned
    */
    gl::GLint clearBufferDrawBuffer(size_t index) const;

    /**
    *  @brief
    *    Get the symbolic constant for the attachment name given a color attachment index
    *
    *  @param[in] index
    *    The color attachment index
    *
    *  @return
    *    The symbolic constant for the attachment name
    */
    gl::GLenum drawBufferAttachment(size_t index) const;


protected:
    RenderTargetType                   m_currentTargetType;        ///< Target type
    AttachmentType                     m_internalAttachmentType;   ///< Internal OpenGL attachment type
    gl::GLenum                         m_defaultFBOAttachment;     ///< Default framebuffer attachment target
    globjects::Texture               * m_texture;                  ///< Texture target
    globjects::Renderbuffer          * m_renderbuffer;             ///< Renderbuffer target
    globjects::FramebufferAttachment * m_userDefinedFBOAttachment; ///< User-defined framebuffer attachment target
};


} // namespace gloperate
