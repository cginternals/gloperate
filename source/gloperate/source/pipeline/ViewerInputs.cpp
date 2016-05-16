
#include <gloperate/pipeline/ViewerInputs.h>


namespace gloperate
{


ViewerInputs::ViewerInputs(ViewerContext * viewerContext, Pipeline * parent)
: Stage(viewerContext, "Input", parent)
, deviceViewport (this, "deviceViewport",  glm::vec4(0, 0, 0, 0))
, virtualViewport(this, "virtualViewport", glm::vec4(0, 0, 0, 0))
, backgroundColor(this, "backgroundColor", glm::vec3(1.0, 1.0, 1.0))
, frameCounter   (this, "frameCounter",    0)
, timeDelta      (this, "timeDelta",       0.0f)
{
}

ViewerInputs::~ViewerInputs()
{
}


} // namespace gloperate
