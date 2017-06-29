
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{
    class ProgramStage;
    class RenderPassStage;
    class RasterizationStage;
    class ClearStage;
    class ShapeStage;
    class TrackballStage;
    class TransformStage;
    class ColorGradientStage;
    class ColorGradientSelectionStage;
    class ColorGradientTextureStage;
}


/**
*  @brief
*    Demo pipeline demonstrating the use of color gradients
*/
class ColorGradientDemo : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientDemo, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline demonstrating the use of color gradients"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<cppassist::FilePath> colors;   ///< Color gradients filename
    Input<std::string>         gradient; ///< Selected gradient
    Input<float>               value;    ///< Value (0..1) that is mapped to color and size


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
    ColorGradientDemo(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorGradientDemo();


protected:
    // Stages
    std::unique_ptr<gloperate::TrackballStage>              m_trackball;              ///< Trackball camera navigation stage

    std::unique_ptr<gloperate::ShapeStage>                  m_shape;                  ///< Stage that generates a basic shape

    std::unique_ptr<gloperate::ColorGradientStage>          m_colorGradientLoading;   ///< Loads color gradients from a gradients files
    std::unique_ptr<gloperate::ColorGradientTextureStage>   m_colorGradientTexture;   ///< Creates a texture from all gradients
    std::unique_ptr<gloperate::ColorGradientSelectionStage> m_colorGradientSelection; ///< Select one gradient from the list of gradients

    std::unique_ptr<gloperate::TransformStage>              m_shapeTransform;         ///< Scales the shape
    std::unique_ptr<gloperate::ProgramStage>                m_shapeProgram;           ///< Builds the Program for rendering the shape
    std::unique_ptr<gloperate::RenderPassStage>             m_shapeRenderPass;        ///< Builds the RenderPass for rendering the shape

    std::unique_ptr<gloperate::ClearStage>                  m_clear;                  ///< Clears the output image
    std::unique_ptr<gloperate::RasterizationStage>          m_shapeRasterization;     ///< Executes the RenderPass
};
