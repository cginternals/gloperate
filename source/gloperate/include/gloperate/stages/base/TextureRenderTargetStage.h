
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class ColorRenderTarget;
class DepthRenderTarget;
class StencilRenderTarget;


/**
*  @brief
*    Stage that creates an empty texture with a specified size and format as render target
*/
class GLOPERATE_API TextureRenderTargetStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TextureRenderTargetStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates an empty texture with a specified size and format as render target"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gl::GLenum> internalFormat; ///< OpenGL internal image format
    Input<gl::GLenum> format;         ///< OpenGL image format
    Input<gl::GLenum> type;           ///< OpenGL data type
    Input<glm::vec4>  size;           ///< Viewport size (only z and w component is used as width and height)

    // Outputs
    Output<globjects::Texture *>             texture;             ///< Texture
    Output<gloperate::ColorRenderTarget *>   colorRenderTarget;   ///< Color RenderTarget
    Output<gloperate::DepthRenderTarget *>   depthRenderTarget;   ///< Depth RenderTarget
    Output<gloperate::StencilRenderTarget *> stencilRenderTarget; ///< Stencil RenderTarget


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
    TextureRenderTargetStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureRenderTargetStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<globjects::Texture>             m_texture;             ///< The created texture
    std::unique_ptr<gloperate::ColorRenderTarget>   m_colorRenderTarget;   ///< The color render target
    std::unique_ptr<gloperate::DepthRenderTarget>   m_depthRenderTarget;   ///< The depth render target
    std::unique_ptr<gloperate::StencilRenderTarget> m_stencilRenderTarget; ///< The stencil render target
};


} // namespace gloperate
