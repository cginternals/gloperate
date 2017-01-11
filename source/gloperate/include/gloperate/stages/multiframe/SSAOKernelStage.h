#pragma once


#include <memory>

#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec2.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


/**
*  @brief
*    Stage that provides kernel and noise for SSAO effect
*/
class GLOPERATE_API SSAOKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SSAOKernelStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides kernel and noise for SSAO effect"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<bool> enable;         ///< Regenerate kernel (optional, default: true)
    Input<int> kernelSize;      ///< Size of SSAO kernel
    Input<int> noiseSize;       ///< Size of SSAO noise
    Input<int> currentFrame;    ///< Number of currently aggregated frame

    // Outputs
    Output<globjects::Texture *> kernelTexture;  ///< Kernel texture
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
    SSAOKernelStage(gloperate::Environment * environment, const std::string & name = "SSAOKernelStage");


protected:
    // Virtual Stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void recreateKernels();

protected:
    globjects::ref_ptr<globjects::Texture> m_kernelTexture;
    globjects::ref_ptr<globjects::Texture> m_noiseTexture;
};


} // namespace gloperate
