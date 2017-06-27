
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/rendering/Shape.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{
    class BasicFramebufferStage;
    class FramebufferStage;
    class TextureLoadStage;
    class TextureStage;
    class ProgramStage;
    class RenderPassStage;
    class RasterizationStage;
    class ClearStage;
    class ScreenAlignedQuad;
    class Quad;
    class ShapeStage;
    class TrackballStage;
    class TimerStage;
    class TransformStage;
    class ColorGradientStage;
    class ColorGradientSelectionStage;
    class ColorGradientTextureStage;
}


/**
*  @brief
*    Demo pipeline displaying a rotating shape
*/
class ColorDemo : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorDemo, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline displaying a rotating shape"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<gloperate::ShapeType> shape;     ///< Shape type
    Input<cppassist::FilePath>  texture;   ///< Texture filename
    Input<float>                angle;     ///< Current rotation angle
    Input<bool>                 rotate;    ///< Rotation automatically?
    Input<gloperate::Color>     color;     ///< Mixer color
    Input<cppassist::FilePath>  colorFile; ///< Color gradients filename
    Input<std::string>          gradient;  ///< Selected gradient


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
    ColorDemo(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorDemo();


protected:
    void onRotateChanged(const bool & rotate);
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;


protected:
    // Stages
    std::unique_ptr<gloperate::TimerStage>                  m_timer;                  ///< Timer for continuous rendering and animation

    std::unique_ptr<gloperate::TrackballStage>              m_trackball;              ///< Trackball camera navigation stage

    std::unique_ptr<gloperate::ShapeStage>                  m_shape;                  ///< Stage that generates a basic shape
    std::unique_ptr<gloperate::TextureLoadStage>            m_texture;                ///< Stage that loads a static picture
    std::unique_ptr<gloperate::BasicFramebufferStage>       m_framebuffer;            ///< Framebuffer for rendering the shape

    std::unique_ptr<gloperate::ClearStage>                  m_clear;                  ///< Clears the output image

    std::unique_ptr<gloperate::TransformStage>              m_shapeTransform;         ///< Rotates the shape around its axis
    std::unique_ptr<gloperate::ProgramStage>                m_shapeProgram;           ///< Builds the Program for rendering the shape
    std::unique_ptr<gloperate::RenderPassStage>             m_shapeRenderPass;        ///< Builds the RenderPass for rendering the shape
    std::unique_ptr<gloperate::RasterizationStage>          m_shapeRasterization;     ///< Executes the RenderPass

    std::unique_ptr<gloperate::ColorGradientStage>          m_colorGradientLoading;   ///< Loads color gradients from a gradients files
    std::unique_ptr<gloperate::ColorGradientSelectionStage> m_colorGradientSelection; ///< Loads color gradients from a gradients files
    std::unique_ptr<gloperate::ColorGradientTextureStage>   m_colorGradientTexture;   ///< Loads color gradients from a gradients files

    std::unique_ptr<gloperate::ProgramStage>                m_colorizeProgram;        ///< Builds the Program for blending
    std::unique_ptr<gloperate::RenderPassStage>             m_colorizeRenderPass;     ///< Builds the RenderPass for blending
    std::unique_ptr<gloperate::RasterizationStage>          m_colorizeRasterization;  ///< Executes the RenderPass

    // Internal data
    std::unique_ptr<gloperate::Quad>                        m_quad;                   ///< Screen-aligned quad for colorization in demo
};
