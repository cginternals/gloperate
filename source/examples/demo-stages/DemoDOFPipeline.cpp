
#include "DemoDOFPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/MultiFrameDiscDistributionStage.h>

#include "DemoDOFCubeStage.h"


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoDOFPipeline, gloperate::Stage)


DemoDOFPipeline::DemoDOFPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_dofShiftStage(cppassist::make_unique<MultiFrameDiscDistributionStage>(environment))
, m_cubeStage(cppassist::make_unique<DemoDOFCubeStage>(environment))
{
    addStage(m_dofShiftStage.get());
    m_dofShiftStage->currentMultiFrame << renderInterface.frameCounter;
    m_dofShiftStage->radius.setValue(0.03f);

    addStage(m_cubeStage.get());
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
