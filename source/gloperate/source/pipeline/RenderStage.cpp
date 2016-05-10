
#include <gloperate/pipeline/RenderStage.h>


namespace gloperate
{


RenderStage::RenderStage(ViewerContext * viewerContext)
: Stage(viewerContext)
, deviceViewport (this, "deviceViewport")
, virtualViewport(this, "virtualViewport")
, backgroundColor(this, "backgroundColor")
, frameCounter   (this, "frameCounter")
, timeDelta      (this, "timeDelta")
, redrawNeeded   (this, "redrawNeeded")
{
}

RenderStage::~RenderStage()
{
}


} // namespace gloperate
