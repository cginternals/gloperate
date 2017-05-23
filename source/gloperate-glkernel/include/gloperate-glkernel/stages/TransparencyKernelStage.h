
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
*    Stage that creates a transparency mask texture for multiframe rendering
*/
class GLOPERATE_GLKERNEL_API TransparencyKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TransparencyKernelStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a transparency mask texture for multiframe rendering"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    gloperate::Input<glm::ivec2> kernelSize;                ///< Size of the kernel, height determines number of alpha steps
    gloperate::Input<bool> regenerate;                      ///< Regenerate kernel?

    // Outputs
    gloperate::Output<std::vector<unsigned char> *> kernel; ///< Pointer to std::vector with kernel values
    gloperate::Output<globjects::Texture *> texture;        ///< Pointer to globjects::Texture with kernel values


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
    TransparencyKernelStage(gloperate::Environment * environment, const std::string & name = "TransparencyKernelStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TransparencyKernelStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper function
    void regenerateKernel();

protected:
    // Data
    std::vector<unsigned char> m_kernelData;          ///< Vector with kernel data
    std::unique_ptr<globjects::Texture> m_texture;    ///< Texture with kernel data


};


} // namespace gloperate_glkernel
