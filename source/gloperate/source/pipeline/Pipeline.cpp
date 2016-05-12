
#include <gloperate/pipeline/Pipeline.h>


namespace gloperate
{


Pipeline::Pipeline(ViewerContext * viewerContext, Stage * parent)
: Stage(viewerContext, parent)
{
}

Pipeline::~Pipeline()
{
}


} // namespace gloperate
