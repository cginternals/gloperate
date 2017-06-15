
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
}


namespace gloperate
{


class AbstractDrawable;


/**
*  @brief
*    Stage that rasterizes a given drawable
*/
class GLOPERATE_API RasterizationStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        RasterizationStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that rasterizes a given drawable"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface                      renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<bool>                          rasterize;       ///< If connected, it enables/disables rasterization
    Input<gloperate::AbstractDrawable *> drawable;        ///< Drawable that is rendered

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
    RasterizationStage(Environment * environment, const std::string & name = "RasterizationStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RasterizationStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;
    virtual void onContextDeinit(AbstractGLContext * content) override;


protected:
    std::unique_ptr<globjects::Framebuffer> m_defaultFBO; ///< Default FBO for clearing
    std::unique_ptr<globjects::Framebuffer> m_fbo;        ///< Intermediate FBO for clearing
};


} // namespace gloperate
