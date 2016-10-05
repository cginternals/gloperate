
#include <gloperate-glheadless/stages/demos/DemoOffscreenPipeline.h>

#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/demos/DemoStage.h>


using namespace gloperate;


namespace gloperate_glheadless {


CPPEXPOSE_COMPONENT(DemoOffscreenPipeline, gloperate::Stage)


DemoOffscreenPipeline::DemoOffscreenPipeline(Environment * environment, const std::string & name)
: OffscreenPipeline(environment, name)
, viewport("viewport", this)
, backgroundColor("backgroundColor", this)
, timeDelta("timeDelta", this)
, colorTexture("colorTexture", this)
, rendered("rendered", this)
, m_framebufferStage(new BasicFramebufferStage(environment, "FramebufferStage"))
, m_renderStage(new DemoStage(environment))
{
    // Framebuffer stage
    addStage(m_framebufferStage);
    m_framebufferStage->viewport << viewport;

    // Demo render stage
    addStage(m_renderStage);
    m_renderStage->renderInterface.deviceViewport  << viewport;
    m_renderStage->renderInterface.targetFBO       << m_framebufferStage->fbo;
    m_renderStage->renderInterface.timeDelta       << timeDelta;
    m_renderStage->renderInterface.backgroundColor << backgroundColor;

    // Outputs
    colorTexture << m_framebufferStage->colorTexture;
    rendered     << m_renderStage->renderInterface.rendered;
}


DemoOffscreenPipeline::~DemoOffscreenPipeline()
{
}


} // namespace gloperate_glheadless
