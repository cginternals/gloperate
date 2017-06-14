
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate
{


CanvasInterface::CanvasInterface(Stage * stage)
: viewport       ("viewport", stage)
, backgroundColor("backgroundColor", stage)
, frameCounter   ("frameCounter", stage)
, timeDelta      ("timeDelta", stage)
{
    // Hide inputs in property editor
    viewport       .setOption("hidden", true);
    backgroundColor.setOption("hidden", true);
    frameCounter   .setOption("hidden", true);
    timeDelta      .setOption("hidden", true);
}

CanvasInterface::~CanvasInterface()
{
}


} // namespace gloperate
