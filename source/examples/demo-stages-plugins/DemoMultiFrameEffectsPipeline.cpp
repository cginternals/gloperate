
#include "DemoMultiFrameEffectsPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/navigation/TrackballStage.h>

#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "MultiFrameRenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoMultiFrameEffectsPipeline, gloperate::Stage)


DemoMultiFrameEffectsPipeline::DemoMultiFrameEffectsPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 256)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_renderingPipeline(cppassist::make_unique<MultiFrameRenderingPipeline>(environment))
, m_trackballStage(cppassist::make_unique<gloperate::TrackballStage>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->addStage(m_renderingPipeline.get());
    m_renderingPipeline->multiFrameCount << multiFrameCount;

    addStage(m_trackballStage.get());
    m_trackballStage->viewport << canvasInterface.viewport;

    // Inputs
    m_renderingPipeline->camera << m_trackballStage->camera;
    m_multiFramePipeline->restartAggregationOn(&m_trackballStage->camera);

    auto useAntialiasingInput = createInput<bool>("useAntialiasing");
    auto useDOFInput = createInput<bool>("useDOF");
    auto useSSAOInput = createInput<bool>("useSSAO");
    auto useTransparencyInput = createInput<bool>("useTransparency");
    m_renderingPipeline->useAntialiasing << *useAntialiasingInput;
    m_renderingPipeline->useDOF          << *useDOFInput;
    m_renderingPipeline->useSSAO         << *useSSAOInput;
    m_renderingPipeline->useTransparency << *useTransparencyInput;
    m_multiFramePipeline->restartAggregationOn(useAntialiasingInput);
    m_multiFramePipeline->restartAggregationOn(useDOFInput);
    m_multiFramePipeline->restartAggregationOn(useSSAOInput);
    m_multiFramePipeline->restartAggregationOn(useTransparencyInput);

    auto dofIntensityInput = createInput<float>("dofIntensity", 0.01f);
    auto dofFocusInput = createInput<float>("dofFocus", 0.1f);
    auto ssaoRadiusInput = createInput<float>("ssaoRadius", 0.5f);
    auto transparencyAlphaInput = createInput<float>("transparencyAlpha", 0.65f);
    m_renderingPipeline->dofIntensity << *dofIntensityInput;
    m_renderingPipeline->dofFocus << *dofFocusInput;
    m_renderingPipeline->ssaoRadius << *ssaoRadiusInput;
    m_renderingPipeline->transparencyAlpha << *transparencyAlphaInput;
    m_multiFramePipeline->restartAggregationOn(dofIntensityInput);
    m_multiFramePipeline->restartAggregationOn(dofFocusInput);
    m_multiFramePipeline->restartAggregationOn(ssaoRadiusInput);
    m_multiFramePipeline->restartAggregationOn(transparencyAlphaInput);

    dofIntensityInput->setOption("maximumValue", 0.02f);
    dofFocusInput->setOption("maximumValue", 2.0f);

    *m_multiFramePipeline->canvasInterface.colorRenderTargetInput(0) << *createInput<gloperate::ColorRenderTarget *>("Color");

    m_multiFramePipeline->canvasInterface.viewport << canvasInterface.viewport;
    m_multiFramePipeline->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_multiFramePipeline->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_multiFramePipeline->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_multiFramePipeline->multiFrameCount << multiFrameCount;

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_multiFramePipeline->canvasInterface.colorRenderTargetOutput(0);
}

DemoMultiFrameEffectsPipeline::~DemoMultiFrameEffectsPipeline()
{
}
