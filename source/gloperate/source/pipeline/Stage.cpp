
#include <gloperate/pipeline/Stage.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext)
: deviceViewport (this, "deviceViewport")
, virtualViewport(this, "virtualViewport")
, backgroundColor(this, "backgroundColor")
, frameCounter   (this, "frameCounter")
, timeDelta      (this, "timeDelta")
, redrawNeeded   (this, "redrawNeeded")
, m_viewerContext(viewerContext)
{
}

Stage::~Stage()
{
}

ViewerContext * Stage::viewerContext() const
{
    return m_viewerContext;
}

void Stage::initContext(AbstractGLContext * context)
{
    onContextInit(context);
}

void Stage::deinitContext(AbstractGLContext * context)
{
    onContextDeinit(context);
}

void Stage::process(AbstractGLContext * context)
{
    onProcess(context);
}

void Stage::onContextInit(AbstractGLContext *)
{
}

void Stage::onContextDeinit(AbstractGLContext *)
{
}

void Stage::onProcess(AbstractGLContext *)
{
}


} // namespace gloperate
