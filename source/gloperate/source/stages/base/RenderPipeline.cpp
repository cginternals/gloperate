
#include <gloperate/stages/base/RenderPipeline.h>


namespace gloperate
{


RenderPipeline::RenderPipeline(ViewerContext * viewerContext, const std::string & name)
: Pipeline(viewerContext, name)
, deviceViewport ("deviceViewport", this)
, virtualViewport("virtualViewport", this)
, backgroundColor("backgroundColor", this)
, frameCounter   ("frameCounter", this)
, timeDelta      ("timeDelta", this)
, targetFBO      ("targetFBO", this)
, rendered       ("rendered", this)
{
}

RenderPipeline::~RenderPipeline()
{
}


} // namespace gloperate
