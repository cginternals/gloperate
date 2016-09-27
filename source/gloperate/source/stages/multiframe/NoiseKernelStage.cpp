
#include <gloperate/stages/multiframe/NoiseKernelStage.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate
{


NoiseKernelStage::NoiseKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, inputDimensions("inputDimensions", this)
, outputDimensions("outputDimensions", this)
, size("size", this)
, noiseTexture("noiseTexture", this)
{
}

void NoiseKernelStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_noiseTexture.reset(new NoiseTexture((*inputDimensions), (*outputDimensions), (*size)));
    
    (*noiseTexture) = m_noiseTexture->texture();
}

void NoiseKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    // invalidateOutputs();
}


} // namespace gloperate
