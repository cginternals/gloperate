#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Parameter.h>

#include <gloperate/rendering/NoiseTexture.h>


namespace globjects
{

class Texture;

}


namespace gloperate
{


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
    NoiseKernelStage(gloperate::Environment * environment, const std::string & name = "NoiseKernelStage");

public:
    Parameter<int> inputDimensions;
    Parameter<int> outputDimensions;
    Parameter<int> size;

    Output<globjects::Texture *> noiseTexture;

protected:
    std::unique_ptr<NoiseTexture> m_noiseTexture;

    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
};


} // namespace gloperate
