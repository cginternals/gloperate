
#include <gloperate/pipeline/ViewerContainer.h>


namespace gloperate
{


ViewerContainer::ViewerContainer(ViewerContext * viewerContext)
: Pipeline(viewerContext, "Root", nullptr)
, inputs(viewerContext, this)
, outputs(viewerContext, this)
{
}

ViewerContainer::~ViewerContainer()
{
}


} // namespace gloperate
