
#include "DOFRenderingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include "DOFCubeStage.h"


CPPEXPOSE_COMPONENT(DOFRenderingPipeline, gloperate::Stage)


DOFRenderingPipeline::DOFRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_dofShiftStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_cubeStage(cppassist::make_unique<DOFCubeStage>(environment))
{
    addStage(m_dofShiftStage.get());
    m_dofShiftStage->kernelSize << multiFrameCount;
    m_dofShiftStage->radius.setValue(0.03f);

    addStage(m_cubeStage.get());
    m_cubeStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_cubeStage->canvasInterface.viewport << canvasInterface.viewport;
    m_cubeStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_cubeStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_cubeStage->dofShifts << m_dofShiftStage->kernel;
    m_cubeStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_cubeStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

DOFRenderingPipeline::~DOFRenderingPipeline()
{
}
