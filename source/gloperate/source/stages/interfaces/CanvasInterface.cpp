
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate
{


CanvasInterface::CanvasInterface(Stage * stage)
: RenderInterface(stage)
, backgroundColor("backgroundColor", stage, Color(0, 0, 0, 255))
, frameCounter   ("frameCounter", stage)
, timeDelta      ("timeDelta", stage)
{
    // Hide inputs in property editor
    //backgroundColor.setOption("hidden", true);
    //frameCounter   .setOption("hidden", true);
    //timeDelta      .setOption("hidden", true);
}

CanvasInterface::~CanvasInterface()
{
}


} // namespace gloperate
