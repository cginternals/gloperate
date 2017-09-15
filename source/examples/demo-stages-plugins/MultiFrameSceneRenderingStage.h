
#pragma once


#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/rendering/Drawable.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


/**
*  @brief
*    Demo stage that renders a simple scene onto the screen
*/
class MultiFrameSceneRenderingStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameSceneRenderingStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that renders a simple scene onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface;            ///< Interface for rendering into a viewer

    // Inputs
    Input<std::vector<glm::vec2> *> subpixelShiftKernel;   ///< Subpixel shifts for antialiasing
    Input<std::vector<glm::vec2> *> dofShiftKernel;        ///< DOF shifts
    Input<globjects::Texture *> noiseKernelTexture;        ///< Noise kernel for transparency
    Input<globjects::Texture *> transparencyKernelTexture; ///< Transparency kernel

    // Outputs
    Output<glm::mat4> projectionMatrix;                    ///< Projection matrix used for rendering
    Output<glm::mat3> normalMatrix;                        ///< Normal matrix used for rendering


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
    MultiFrameSceneRenderingStage(gloperate::Environment * environment, const std::string & name = "MultiFrameSceneRenderingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameSceneRenderingStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

    // Helper functions
    void setupGeometry();
    void setupProgram();


protected:
    // Rendering objects
    std::unique_ptr<gloperate::Drawable> m_drawable;
    std::unique_ptr<globjects::Shader>   m_vertexShader;
    std::unique_ptr<globjects::Shader>   m_fragmentShader;
    std::unique_ptr<globjects::Program>  m_program;
};
