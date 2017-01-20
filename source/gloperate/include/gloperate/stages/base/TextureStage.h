
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

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
    class RenderTarget;
}


namespace gloperate
{


/**
*  @brief
*    Stage that creates an empty texture with a specified size and format
*/
class GLOPERATE_API TextureStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TextureStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates an empty texture with a specified size and format"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gl::GLenum> internalFormat;     ///< OpenGL internal image format
    Input<gl::GLenum> format;             ///< OpenGL image format
    Input<gl::GLenum> type;               ///< OpenGL data type
    Input<glm::vec4>  size;               ///< Viewport size

    // Outputs
    Output<globjects::Texture *> texture;           ///< Texture
    Output<gloperate::RenderTarget *> renderTarget; ///< RenderTarget


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
    TextureStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;


protected:
    // Data
    globjects::ref_ptr<globjects::Texture> m_texture; ///< The created texture
    globjects::ref_ptr<gloperate::RenderTarget> m_renderTarget; ///< The passed render target
};


} // namespace gloperate
