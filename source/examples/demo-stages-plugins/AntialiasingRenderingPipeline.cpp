
#include "AntialiasingRenderingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include "AntialiasableTriangleStage.h"


CPPEXPOSE_COMPONENT(AntialiasingRenderingPipeline, gloperate::Stage)


AntialiasingRenderingPipeline::AntialiasingRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_triangleStage(cppassist::make_unique<AntialiasableTriangleStage>(environment))
{
    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    m_subpixelStage->radius.setValue(0.001f); // guessing inverse height of viewport

    addStage(m_triangleStage.get());
    m_triangleStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_triangleStage->canvasInterface.viewport << canvasInterface.viewport;
    m_triangleStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_triangleStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_triangleStage->subpixelOffsets << m_subpixelStage->kernel;

    m_triangleStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    *createOutput<gloperate::ColorRenderTarget *>("Color") << *m_triangleStage->createOutput<gloperate::ColorRenderTarget *>("Color");
}

AntialiasingRenderingPipeline::~AntialiasingRenderingPipeline()
{
}
