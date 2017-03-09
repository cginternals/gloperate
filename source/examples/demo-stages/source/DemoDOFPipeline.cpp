
#include <demo-stages/DemoDOFPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include <demo-stages/DemoDOFCubeStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoDOFPipeline, gloperate::Stage)


DemoDOFPipeline::DemoDOFPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_dofShiftStage(new gloperate_glkernel::DiscDistributionKernelStage(environment))
, m_cubeStage(new DemoDOFCubeStage(environment))
{
    addStage(m_dofShiftStage);
    m_dofShiftStage->kernelSize << multiFrameCount;
    m_dofShiftStage->radius.setValue(0.03f);

    addStage(m_cubeStage);
    m_cubeStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_cubeStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_cubeStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_cubeStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_cubeStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_cubeStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_cubeStage->dofShifts << m_dofShiftStage->kernel;

    renderInterface.rendered << m_cubeStage->renderInterface.rendered;
}

DemoDOFPipeline::~DemoDOFPipeline()
{
}


} // namespace gloperate
