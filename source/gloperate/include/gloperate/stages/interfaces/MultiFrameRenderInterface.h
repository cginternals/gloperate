
#pragma once


#include <gloperate/base/GlmProperties.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{

class Texture;

}

namespace gloperate
{


class MultiFrameRenderInterface : public RenderInterface
{
public:
    // Inputs
    Input<globjects::Texture *> noiseKernel;
    Input<globjects::Texture *> ssaoKernel;
    Input<globjects::Texture *> ssaoNoise;
    Input<globjects::Texture *> transparencyKernel;
    Input<glm::vec2> dofShift;
    Input<glm::vec2> subpixelOffset;

    // Outputs
    Output<bool> reprocessSSAO;
    Output<bool> reprocessTransparency;
    Output<bool> reprocessDOFShift;
    Output<int> noiseInputDimensions;
    Output<int> noiseOutputDimensions;
    Output<int> noiseKernelSize;
    Output<float> dofRadius;


public:
    MultiFrameRenderInterface(Stage * stage);

    ~MultiFrameRenderInterface();

};


} // namespace gloperate
