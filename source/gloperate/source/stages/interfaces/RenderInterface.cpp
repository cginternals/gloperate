
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: deviceViewport ("deviceViewport", stage)
, virtualViewport("virtualViewport", stage)
, backgroundColor("backgroundColor", stage)
, frameCounter   ("frameCounter", stage)
, timeDelta      ("timeDelta", stage)
, renderTarget   ("renderTarget", stage)
, renderTargetOut("renderTargetOut", stage)
{
    // Hide inputs in property editor
    deviceViewport .setOption("hidden", true);
    virtualViewport.setOption("hidden", true);
    backgroundColor.setOption("hidden", true);
    frameCounter   .setOption("hidden", true);
    timeDelta      .setOption("hidden", true);
    renderTarget   .setOption("hidden", true);
    renderTargetOut.setOption("hidden", true);
}

RenderInterface::~RenderInterface()
{
}


} // namespace gloperate
