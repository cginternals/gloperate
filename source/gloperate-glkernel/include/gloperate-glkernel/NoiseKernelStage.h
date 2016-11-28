
#pragma once

#include <gloperate-glkernel/gloperate-glkernel_api.h>

#include <glkernel/Kernel.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

namespace globjects
{
    class Texture;
}


namespace gloperate_glkernel
{


class GLOPERATE_GLKERNEL_API NoiseKernelStage : public gloperate::Stage
{

public:
    NoiseKernelStage(gloperate::Environment * environment);

public:
    gloperate::Input<glm::ivec3> dimensions;

    gloperate::Output<glkernel::kernel3> kernel;
    gloperate::Output<globjects::ref_ptr<globjects::Texture>> texture;

protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

protected:
    void resizeKernel();
    void regenerateKernel();

protected:
    glkernel::kernel3 m_kernel;

    globjects::Texture * m_texture;

};


}
