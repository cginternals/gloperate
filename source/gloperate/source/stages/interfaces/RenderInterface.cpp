
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
    // Hide inputs in property editor
    deviceViewport .setOption("hidden", true);
    virtualViewport.setOption("hidden", true);
    backgroundColor.setOption("hidden", true);
    frameCounter   .setOption("hidden", true);
    virtualTime    .setOption("hidden", true);
    timeDelta      .setOption("hidden", true);
    targetFBO      .setOption("hidden", true);
    rendered       .setOption("hidden", true);
}

RenderInterface::~RenderInterface()
{
}


} // namespace gloperate
