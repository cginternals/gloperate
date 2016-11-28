
#pragma once

#include <gloperate-glkernel/gloperate-glkernel_api.h>

#include <memory>

#include <glkernel/Kernel.h>

#include <globjects/base/ref_ptr.h>

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
    DiscDistributionKernelStage(gloperate::Environment * environment);

public:
    gloperate::Input<int> kernelSize;
    gloperate::Input<bool> regenerate;

    gloperate::Output<glkernel::kernel2> kernel;
    gloperate::Output<globjects::ref_ptr<globjects::Texture>> texture;

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
