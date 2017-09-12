
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace globjects
{
    class NamedString;
}


namespace gloperate
{
    class Camera;

    class LightCreationStage;
    class LightBufferTextureStage;
    class TimerStage;
    class TextureRenderTargetStage;
    class ShapeStage;
    class TransformStage;
    class ClearStage;
    class ProgramStage;
    class RenderPassStage;
    class RasterizationStage;
}


/**
*  @brief
*    Pipeline that renders a rotating cube illuminated by three light sources
*/
class LightTestPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightTestPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Stage that renders a rotating cube illuminated by three light sources"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<float> glossiness;                    ///< Glossiness of the cube (0.0 to 1.0)

    Input<int> lightType1;                      ///< Type of 1st light
    Input<int> lightType2;                      ///< Type of 2nd light
    Input<int> lightType3;                      ///< Type of 3rd light

    Input<glm::vec3> lightColor1;               ///< Color of 1st light
    Input<glm::vec3> lightColor2;               ///< Color of 2nd light
    Input<glm::vec3> lightColor3;               ///< Color of 3rd light

    Input<glm::vec3> lightPos1;                 ///< Position/Direction of 1st light
    Input<glm::vec3> lightPos2;                 ///< Position/Direction of 2nd light
    Input<glm::vec3> lightPos3;                 ///< Position/Direction of 3rd light

    Input<glm::vec3> lightAttenuation1;         ///< Attenuation coefficients of 1st light
    Input<glm::vec3> lightAttenuation2;         ///< Attenuation coefficients of 2nd light
    Input<glm::vec3> lightAttenuation3;         ///< Attenuation coefficients of 3rd light


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
    LightTestPipeline(gloperate::Environment * environment, const std::string & name = "LightTestPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~LightTestPipeline();


protected:
    // Virtual Stage interface
    void onContextInit(gloperate::AbstractGLContext * context);
    void onContextDeinit(gloperate::AbstractGLContext * context);

protected:
    // Stages
    std::unique_ptr<gloperate::LightCreationStage>       m_lightDefStage1;
    std::unique_ptr<gloperate::LightCreationStage>       m_lightDefStage2;
    std::unique_ptr<gloperate::LightCreationStage>       m_lightDefStage3;
    std::unique_ptr<gloperate::LightBufferTextureStage>  m_lightAccumulationStage;
    std::unique_ptr<gloperate::TimerStage>               m_timerStage;
    std::unique_ptr<gloperate::TextureRenderTargetStage> m_depthBufferStage;
    std::unique_ptr<gloperate::ClearStage>               m_clearStage;
    std::unique_ptr<gloperate::ProgramStage>             m_programStage;
    std::unique_ptr<gloperate::ShapeStage>               m_shapeStage;
    std::unique_ptr<gloperate::TransformStage>           m_transformStage;
    std::unique_ptr<gloperate::RenderPassStage>          m_renderPassStage;
    std::unique_ptr<gloperate::RasterizationStage>       m_rasterizationStage;

    // Rendering ressources
    std::unique_ptr<gloperate::Camera>      m_camera;
    std::unique_ptr<globjects::NamedString> m_lightProcessingString;
    std::unique_ptr<globjects::NamedString> m_lightProcessingDiffuseString;
    std::unique_ptr<globjects::NamedString> m_lightProcessingPhongString;
};
