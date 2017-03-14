
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/gl.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{


class Program;
class Framebuffer;
class Texture;


} // namespace globjects


namespace gloperate
{


class Drawable;
class RenderPass;


/**
*  @brief
*    Stage that rasterizes a scene, given as RenderPass or Drawable + Program
*
*    The Drawable and Program are rendered into the given Framebuffer.
*    Alternatively if RenderPass is connected, the RenderPass is used for rasterization.
*/
class GLOPERATE_API RasterizationStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        RasterizationStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that rasterizes a scene, given as RenderPass or Drawable + Program"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface;            ///< Interface for rendering into a viewer

    // Inputs
    Input<bool> rasterize;                      ///< if connected, it enables/disables rasterization
    Input<gloperate::Drawable *> drawable;      ///< the drawable to be drawn
    Input<globjects::Program *> program;        ///< the program used for rendering
    Input<gloperate::RenderPass *> renderPass;  ///< if connected, it replaces program and drawable input

    Input<globjects::Texture *> colorTexture;   ///< pass in of texture input/output

    // Outputs
    Output<globjects::Framebuffer *> fboOut;     ///< pass through framebuffer
    Output<globjects::Texture *> colorTextureOut;///< pass through color texture


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
    virtual void onProcess(AbstractGLContext * context) override;
    void onContextInit(AbstractGLContext * content) override;

protected:
    // protected members
};


} // namespace gloperate
