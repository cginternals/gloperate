
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/rendering/Shape.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


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
    class FloatSelectionStage;
    class TextureFromRenderTargetExtractionStage;
    class ViewportScaleStage;
}


/**
*  @brief
*    Demo pipeline displaying a rotating shape
*/
class ShapeDemo : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShapeDemo, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline displaying a rotating shape"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface  canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<gloperate::ShapeType> shape;           ///< Shape type
    Input<cppassist::FilePath>  texture;         ///< Texture filename
    Input<float>                angle;           ///< Current rotation angle
    Input<bool>                 rotate;          ///< Rotation automatically?
    Input<gloperate::Color>     color;           ///< Mixer color


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
    ShapeDemo(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShapeDemo();


protected:
    /**
    *  @brief
    *    React on rotation type change
    *
    *  @param[in] rotate
    *    Rotation flag
    *
    *  @remarks
    *    A 'rotate' value of 'true' enables automatic rotation, 'false' disables it
    */
    void onRotateChanged(const bool & rotate);

    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;


protected:
    // Stages
    std::unique_ptr<gloperate::TimerStage>                             m_timer;                  ///< Timer for continuous rendering and animation

    std::unique_ptr<gloperate::FloatSelectionStage>                    m_floatSelection;         ///< Selection between user-defined angle and timer-updated angle

    std::unique_ptr<gloperate::TrackballStage>                         m_trackball;              ///< Trackball camera navigation stage

    std::unique_ptr<gloperate::ViewportScaleStage>                     m_viewportScale;          ///< Viewport scale stage

    std::unique_ptr<gloperate::ShapeStage>                             m_shape;                  ///< Stage that generates a basic shape
    std::unique_ptr<gloperate::TextureLoadStage>                       m_texture;                ///< Stage that loads a static picture
    std::unique_ptr<gloperate::BasicFramebufferStage>                  m_framebuffer;            ///< Framebuffer for rendering the shape

    std::unique_ptr<gloperate::ClearStage>                             m_clear;                  ///< Clears the output image

    std::unique_ptr<gloperate::TransformStage>                         m_shapeTransform;         ///< Rotates the shape around its axis
    std::unique_ptr<gloperate::ProgramStage>                           m_shapeProgram;           ///< Builds the Program for rendering the shape
    std::unique_ptr<gloperate::RenderPassStage>                        m_shapeRenderPass;        ///< Builds the RenderPass for rendering the shape
    std::unique_ptr<gloperate::RasterizationStage>                     m_shapeRasterization;     ///< Executes the RenderPass

    std::unique_ptr<gloperate::ProgramStage>                           m_colorizeProgram;        ///< Builds the Program for blending
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage> m_textureExtractionStage; ///< Texture from rendertarget extraction
    std::unique_ptr<gloperate::RenderPassStage>                        m_colorizeRenderPass;     ///< Builds the RenderPass for blending
    std::unique_ptr<gloperate::RasterizationStage>                     m_colorizeRasterization;  ///< Executes the RenderPass

    // Internal data
    std::unique_ptr<gloperate::Quad>                                   m_quad;                   ///< Screen-aligned quad for colorization in demo
};
