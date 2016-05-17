
#include <gloperate/pipeline/ViewerOutputs.h>


namespace gloperate
{


ViewerOutputs::ViewerOutputs(ViewerContext * viewerContext, Pipeline * parent)
: Stage(viewerContext, "Output", parent)
, rendered(this, "rendered", false)
{
}

ViewerOutputs::~ViewerOutputs()
{
}


} // namespace gloperate
