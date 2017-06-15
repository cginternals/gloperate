
#include "MultiFrameRenderingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include "AntialiasableTriangleStage.h"


CPPEXPOSE_COMPONENT(MultiFrameRenderingPipeline, gloperate::Stage)


MultiFrameRenderingPipeline::MultiFrameRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
{
    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    // subpixel shifts are scaled in rendering stage


    //renderInterface.rendered << [rendering stage]->renderInterface.rendered;
}

MultiFrameRenderingPipeline::~MultiFrameRenderingPipeline()
{
}
