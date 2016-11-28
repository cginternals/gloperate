
#pragma once

#include <gloperate-glkernel/gloperate-glkernel_api.h>

#include <glkernel/Kernel.h>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

namespace globjects
{
    class Texture;
}


namespace gloperate_glkernel
{


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
    TransparencyKernelStage(gloperate::Environment * environment, const std::string & name = "Transparency Kernel");

public:
    gloperate::Output<glkernel::kernel1> kernel;
    gloperate::Output<globjects::ref_ptr<globjects::Texture>> texture;

protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

protected:
    globjects::Texture * m_texture;
};


} // namespace gloperate_glkernel
