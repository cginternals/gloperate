
#include <gloperate/stages/base/RenderPipeline.h>


namespace gloperate
{


RenderPipeline::RenderPipeline(ViewerContext * viewerContext, const std::string & name)
: Pipeline(viewerContext, name)
, renderInterface(this)
{
}

RenderPipeline::~RenderPipeline()
{
}


} // namespace gloperate
