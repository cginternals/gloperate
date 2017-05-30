
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: deviceViewport ("deviceViewport", stage)
, virtualViewport("virtualViewport", stage)
, backgroundColor("backgroundColor", stage)
, frameCounter   ("frameCounter", stage)
, virtualTime    ("virtualTime", stage)
, timeDelta      ("timeDelta", stage)
, targetFBO      ("targetFBO", stage)
, rendered       ("rendered", stage)
{
}

RenderInterface::~RenderInterface()
{
}


} // namespace gloperate
