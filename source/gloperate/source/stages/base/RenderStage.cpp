
#include <gloperate/stages/base/RenderStage.h>


namespace gloperate
{


RenderStage::RenderStage(ViewerContext * viewerContext, const std::string & name)
: Stage(viewerContext, name)
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
