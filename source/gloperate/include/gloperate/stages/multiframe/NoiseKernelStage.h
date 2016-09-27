#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>

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
    NoiseKernelStage(gloperate::Environment * environment, const std::string & name = "NoiseKernelStage");

public:
    Input<int> inputDimensions;
    Input<int> outputDimensions;
    Input<int> size;

    Output<globjects::ref_ptr<globjects::Texture>> noiseTexture;

protected:
    std::unique_ptr<NoiseTexture> m_noiseTexture;

    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
};


} // namespace gloperate
