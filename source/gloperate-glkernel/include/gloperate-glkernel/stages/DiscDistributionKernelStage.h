
#pragma once


#include <vector>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <glkernel/Kernel.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that creates a set of random 2D-vectors for multiframe rendering
*/
class GLOPERATE_GLKERNEL_API DiscDistributionKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DiscDistributionKernelStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a set of random 2D-vectors for multiframe rendering"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<int> kernelSize;                   ///< Number of values to generate
    Input<float> radius;                     ///< Radius of the distribution disc
    Input<bool> regenerate;                  ///< Regenerate kernel?

    // Outputs
    Output<std::vector<glm::vec2> *> kernel; ///< Pointer to std::vector with kernel values
    Output<globjects::Texture *> texture;    ///< Pointer to globjects::Texture with kernel values


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
    DiscDistributionKernelStage(gloperate::Environment * environment, const std::string & name = "DiscDistributionKernelStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DiscDistributionKernelStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

    // Helper functions
    void resizeKernel();
    void regenerateKernel();


protected:
    // Data
    glkernel::kernel2 m_kernel;                       ///< Kernel object
    std::vector<glm::vec2> m_kernelData;              ///< Vector with kernel data
    std::unique_ptr<globjects::Texture> m_texture;    ///< Texture with kernel data
};


} // namespace gloperate_glkernel
