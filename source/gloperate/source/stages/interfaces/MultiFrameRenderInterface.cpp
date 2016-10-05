
#include <gloperate/stages/interfaces/MultiFrameRenderInterface.h>


namespace gloperate
{


MultiFrameRenderInterface::MultiFrameRenderInterface(Stage * stage)
: RenderInterface(stage)
, noiseKernel("noiseKernel", stage)
, ssaoKernel("ssaoKernel", stage)
, ssaoNoise("ssaoNoise", stage)
, transparencyKernel("transparencyKernel", stage)
, dofShift("dofShift", stage)
, subpixelOffset("subpixelOffset", stage)
, reprocessSSAO("reprocessSSAO", stage)
, reprocessTransparency("reprocessTransparency", stage)
, reprocessDOFShift("reprocessDOFShift", stage)
, noiseInputDimensions("noiseInputDimensions", stage, 2)
, noiseOutputDimensions("noiseOutputDimensions", stage, 3)
, noiseKernelSize("noiseKernelSize", stage, 16)
, dofRadius("dofRadius", stage)
{
    reprocessSSAO.setFeedback(true);
    reprocessTransparency.setFeedback(true);
    reprocessDOFShift.setFeedback(true);
    noiseInputDimensions.setFeedback(true);
    noiseOutputDimensions.setFeedback(true);
    noiseKernelSize.setFeedback(true);
    dofRadius.setFeedback(true);
}

MultiFrameRenderInterface::~MultiFrameRenderInterface()
{
}


} // namespace gloperate
