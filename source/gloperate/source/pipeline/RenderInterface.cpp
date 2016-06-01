
#include <gloperate/pipeline/RenderInterface.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: deviceViewport ("deviceViewport", stage)
, virtualViewport("virtualViewport", stage)
, backgroundColor("backgroundColor", stage)
, frameCounter   ("frameCounter", stage)
, timeDelta      ("timeDelta", stage)
, targetFBO      ("targetFBO", stage)
, rendered       ("rendered", stage)
{
}

RenderInterface::~RenderInterface()
{
}


} // namespace gloperate
