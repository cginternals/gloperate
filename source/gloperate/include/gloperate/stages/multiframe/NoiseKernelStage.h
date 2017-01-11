#pragma once


#include <memory>

#include <glm/vec2.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/rendering/NoiseTexture.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


/**
*  @brief
*    Stage that provides random noise
*/
class GLOPERATE_API NoiseKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        NoiseKernelStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides random noise"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<int> inputDimensions;                ///< Number of dimensions of noise texture
    Input<int> outputDimensions;               ///< Number of components in noise texture
    Input<int> size;                           ///< Size of noise texture

    // Outputs
    Output<globjects::Texture *> noiseTexture; ///< Noise texture


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
    NoiseKernelStage(gloperate::Environment * environment, const std::string & name = "NoiseKernelStage");


protected:
    // Virtual Stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;

    // Helper Functions
    void recreateNoise();


protected:
    // Data
    std::unique_ptr<NoiseTexture> m_noiseTexture; ///< Noise texture
};


} // namespace gloperate
