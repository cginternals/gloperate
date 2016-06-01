
#include <gloperate/pipeline/RenderPipeline.h>


namespace gloperate
{


RenderPipeline::RenderPipeline(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Pipeline(viewerContext, name, parent)
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
