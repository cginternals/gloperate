
#include <gloperate/stages/base/RenderStage.h>


namespace gloperate
{


RenderStage::RenderStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
, deviceViewport ("deviceViewport", this)
, virtualViewport("virtualViewport", this)
, backgroundColor("backgroundColor", this)
, frameCounter   ("frameCounter", this)
, timeDelta      ("timeDelta", this)
, targetFBO      ("targetFBO", this)
, rendered       ("rendered", this)
{
}

RenderStage::~RenderStage()
{
}


} // namespace gloperate
