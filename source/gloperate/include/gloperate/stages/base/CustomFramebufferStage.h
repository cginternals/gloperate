
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <glbinding/gl/types.h>


namespace gloperate
{


/**
*  @brief
*    Stage that maintains a framebuffer with a color and depth attachment
*/
class GLOPERATE_API CustomFramebufferStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        CustomFramebufferStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that maintains a framebuffer with a color and depth attachment"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Parameters
    Input<gl::GLenum>            format;           ///< Color texture format
    Input<gl::GLenum>            internalFormat;   ///< Internal format for color texture
    Input<gl::GLenum>            dataType;         ///< Color texture data type

    // Inputs
    Input<glm::vec4>                 viewport;     ///< Texture size

    // Outputs
    Output<globjects::Framebuffer *> fbo;          ///< Framebuffer
    Output<globjects::Texture *>     colorTexture; ///< Color attachment
    Output<globjects::Texture *>     depthTexture; ///< Depth attachment


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    CustomFramebufferStage(Environment * environment, const std::string & name = "CustomFramebufferStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~CustomFramebufferStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void rebuildFBO();


protected:
    // Data
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;       ///< The created framebuffer
    globjects::ref_ptr<globjects::Texture>     m_texColor;  ///< Color texture
    globjects::ref_ptr<globjects::Texture>     m_texDepth;  ///< Depth texture
};


} // namespace gloperate
