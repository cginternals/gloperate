#pragma once

#include <memory>

#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec2.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Parameter.h>
#include <gloperate/pipeline/Input.h>


namespace globjects
{

class Texture;

}


namespace gloperate
{


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
    SSAOKernelStage(gloperate::Environment * environment, const std::string & name = "SSAOKernelStage");

public:
    Input<bool> enable;

    Parameter<int> kernelSize;
    Parameter<int> noiseSize;

    Input<int> currentFrame;
    Input<int> multiFrameCount;

    Output<globjects::Texture *> ssaoTexture;
    Output<globjects::Texture *> noiseTexture;

protected:

    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
};


} // namespace gloperate
