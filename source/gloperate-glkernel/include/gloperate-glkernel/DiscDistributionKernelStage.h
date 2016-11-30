
#pragma once

#include <gloperate-glkernel/gloperate-glkernel_api.h>

#include <memory>

#include <glkernel/Kernel.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

namespace globjects
{
    class Texture;
    class Buffer;
}


namespace gloperate_glkernel
{


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
    DiscDistributionKernelStage(gloperate::Environment * environment, const std::string & name = "Disc Distribution Kernel");

public:
    gloperate::Input<int> kernelSize;
    gloperate::Input<bool> regenerate;

    gloperate::Output<glkernel::kernel2> kernel;
    gloperate::Output<globjects::Texture *> texture;

protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

protected:
    void resizeKernel();
    void regenerateKernel();

protected:
    glkernel::kernel2 m_kernel;

    globjects::Texture * m_texture;
    
};


}
