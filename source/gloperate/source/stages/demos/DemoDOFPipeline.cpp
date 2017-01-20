
#include <gloperate/stages/demos/DemoDOFPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/MultiFrameDiscDistributionStage.h>
#include <gloperate/stages/demos/DemoDOFCubeStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoDOFPipeline, gloperate::Stage)


DemoDOFPipeline::DemoDOFPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_dofShiftStage(new MultiFrameDiscDistributionStage(environment))
, m_cubeStage(new DemoDOFCubeStage(environment))
{
    addStage(m_dofShiftStage);
    m_dofShiftStage->currentMultiFrame << renderInterface.frameCounter;
    m_dofShiftStage->radius.setValue(0.03f);

    addStage(m_cubeStage);
    m_cubeStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_cubeStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_cubeStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_cubeStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_cubeStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_cubeStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_cubeStage->dofShift << m_dofShiftStage->value;

    renderInterface.rendered << m_cubeStage->renderInterface.rendered;
}

DemoDOFPipeline::~DemoDOFPipeline()
{
}


} // namespace gloperate
