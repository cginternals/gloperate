
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
, useAntialiasing("useAntialiasing", this)
, useDOF("useDOF", this)
, useSSAO("useSSAO", this)
, useTransparency("useTransparency", this)
, dofIntensity("dofIntensity", this, 0.01f)
, dofZFocus("dofZFocus", this, 0.1f)
, ssaoRadius("ssaoRadius", this, 0.15f)
, ssaoIntensity("ssaoIntensity", this, 0.4f)
, transparencyAlpha("transparencyAlpha", this, 0.65f)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_renderingPipeline(cppassist::make_unique<MultiFrameRenderingPipeline>(environment))
, m_trackballStage(cppassist::make_unique<gloperate::TrackballStage>(environment))
{
    multiFrameCount.setOptions({
        {"type", "int"}, // Workaround: replace auto-assigned value "int32" to display editor
        {"minimumValue", 1},
        {"maximumValue", 4096},
        {"asSpinBox", true}
    });

    m_renderingPipeline->multiFrameCount << multiFrameCount;

    m_trackballStage->viewport << canvasInterface.viewport;

    // Force m_trackballStage to be sorted in front of m_multiFramePipeline
    m_multiFramePipeline->createInput("camera") << m_trackballStage->camera;

    // Inputs
    m_renderingPipeline->camera << m_trackballStage->camera;
    m_multiFramePipeline->restartAggregationOn(&m_trackballStage->camera);

    m_renderingPipeline->useAntialiasing << useAntialiasing;
    m_renderingPipeline->useDOF          << useDOF;
    m_renderingPipeline->useSSAO         << useSSAO;
    m_renderingPipeline->useTransparency << useTransparency;
    m_multiFramePipeline->restartAggregationOn(&useAntialiasing);
    m_multiFramePipeline->restartAggregationOn(&useDOF);
    m_multiFramePipeline->restartAggregationOn(&useSSAO);
    m_multiFramePipeline->restartAggregationOn(&useTransparency);

    m_renderingPipeline->dofIntensity << dofIntensity;
    m_renderingPipeline->dofZFocus << dofZFocus;
    m_renderingPipeline->ssaoRadius << ssaoRadius;
    m_renderingPipeline->ssaoIntensity << ssaoIntensity;
    m_renderingPipeline->transparencyAlpha << transparencyAlpha;
    m_multiFramePipeline->restartAggregationOn(&dofIntensity);
    m_multiFramePipeline->restartAggregationOn(&dofZFocus);
    m_multiFramePipeline->restartAggregationOn(&ssaoRadius);
    m_multiFramePipeline->restartAggregationOn(&ssaoIntensity);
    m_multiFramePipeline->restartAggregationOn(&transparencyAlpha);

    dofIntensity.setOption("maximumValue", 0.02f);
    dofIntensity.setOption("updateOnDrag", true);
    dofZFocus.setOption("maximumValue", 2.0f);
    dofZFocus.setOption("updateOnDrag", true);
    ssaoRadius.setOption("updateOnDrag", true);
    transparencyAlpha.setOption("updateOnDrag", true);

    m_multiFramePipeline->aggregationTarget << *createInput<gloperate::ColorRenderTarget *>("Color");

    m_multiFramePipeline->canvasInterface.viewport << canvasInterface.viewport;
    m_multiFramePipeline->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_multiFramePipeline->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_multiFramePipeline->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_multiFramePipeline->multiFrameCount << multiFrameCount;

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << m_multiFramePipeline->aggregatedTarget;
    // *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_renderingPipeline->canvasInterface.colorRenderTargetOutput(0);

    addStage(m_multiFramePipeline.get());
    m_multiFramePipeline->addStage(m_renderingPipeline.get());
    addStage(m_trackballStage.get());
}

DemoMultiFrameEffectsPipeline::~DemoMultiFrameEffectsPipeline()
{
}
