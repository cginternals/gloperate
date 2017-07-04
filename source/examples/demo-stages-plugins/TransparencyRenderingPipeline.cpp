
#include "TransparencyRenderingPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include "TransparentCirclesStage.h"


CPPEXPOSE_COMPONENT(TransparencyRenderingPipeline, gloperate::Stage)


TransparencyRenderingPipeline::TransparencyRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment))
, m_noiseKernelStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_transparencyRenderStage(cppassist::make_unique<TransparentCirclesStage>(environment))
{
    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize.setValue(glm::ivec2(256, 256));

    addStage(m_noiseKernelStage.get());
    m_noiseKernelStage->dimensions.setValue(glm::ivec3(64, 64, 64));

    addStage(m_transparencyRenderStage.get());
    m_transparencyRenderStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_transparencyRenderStage->canvasInterface.viewport << canvasInterface.viewport;
    m_transparencyRenderStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_transparencyRenderStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_transparencyRenderStage->transparencyKernel << m_transparencyKernelStage->texture;
    m_transparencyRenderStage->noiseKernel << m_noiseKernelStage->texture;
    m_transparencyKernelStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_transparencyRenderStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

TransparencyRenderingPipeline::~TransparencyRenderingPipeline()
{
}
