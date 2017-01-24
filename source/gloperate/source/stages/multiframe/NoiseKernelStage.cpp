
#include <gloperate/stages/multiframe/NoiseKernelStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(NoiseKernelStage, gloperate::Stage)


NoiseKernelStage::NoiseKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, inputDimensions("inputDimensions", this, 2)
, outputDimensions("outputDimensions", this, 3)
, size("size", this, 16)
, noiseTexture("noiseTexture", this)
{
}

void NoiseKernelStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    recreateNoise();
}

void NoiseKernelStage::onProcess(gloperate::AbstractGLContext * /*context*/)
{
    recreateNoise();
}

void NoiseKernelStage::recreateNoise()
{
    m_noiseTexture.reset(new NoiseTexture((*inputDimensions), (*outputDimensions), (*size)));

    noiseTexture.setValue(m_noiseTexture->texture());
}


} // namespace gloperate
