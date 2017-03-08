
#pragma once

#include <gloperate-glkernel/gloperate-glkernel_api.h>

#include <vector>

#include <glkernel/Kernel.h>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that creates a set of random 3D-vectors for multiframe rendering
*/
class GLOPERATE_GLKERNEL_API HemisphereDistributionKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        HemisphereDistributionKernelStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a set of random 3D-vectors for multiframe rendering"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    gloperate::Input<int> kernelSize;                   ///< Number of values to generate
    gloperate::Input<bool> regenerate;                  ///< Regenerate kernel?

    // Outputs
    gloperate::Output<std::vector<glm::vec3> *> kernel; ///< Pointer to std::vector with kernel values
    gloperate::Output<globjects::Texture *> texture;    ///< Pointer to globjects::Texture with kernel values


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
    HemisphereDistributionKernelStage(gloperate::Environment * environment, const std::string & name = "Hemisphere Distribution Kernel");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~HemisphereDistributionKernelStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext *) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void resizeKernel();
    void regenerateKernel();


protected:
    // Data
    glkernel::kernel3 m_kernel;                       ///< Kernel object
    std::vector<glm::vec3> m_kernelData;              ///< Vector with kernel data
    std::unique_ptr<globjects::Texture> m_texture;    ///< Texture with kernel data
    

};


} // namespace gloperate_glkernel
