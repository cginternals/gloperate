
#include <gloperate/stages/base/RenderStage.h>


namespace gloperate
{


RenderStage::RenderStage(ViewerContext * viewerContext, const std::string & name)
: Stage(viewerContext, name)
, renderInterface(this)
{
}

RenderStage::~RenderStage()
{
}


} // namespace gloperate
