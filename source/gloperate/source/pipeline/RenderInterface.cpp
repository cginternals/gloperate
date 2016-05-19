
#include <gloperate/pipeline/RenderInterface.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: deviceViewport (stage, "deviceViewport")
, virtualViewport(stage, "virtualViewport")
, backgroundColor(stage, "backgroundColor")
, frameCounter   (stage, "frameCounter")
, timeDelta      (stage, "timeDelta")
, targetFBO      (stage, "targetFBO")
, rendered       (stage, "rendered")
{
}

RenderInterface::~RenderInterface()
{
}


} // namespace gloperate
