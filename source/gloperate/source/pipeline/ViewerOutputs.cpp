
#include <gloperate/pipeline/ViewerOutputs.h>


namespace gloperate
{


ViewerOutputs::ViewerOutputs(ViewerContext * viewerContext, Pipeline * parent)
: Stage(viewerContext, "Output", parent)
, redrawNeeded(this, "redrawNeeded", false)
{
}

ViewerOutputs::~ViewerOutputs()
{
}


} // namespace gloperate
