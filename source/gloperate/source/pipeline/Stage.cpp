
#include <gloperate/pipeline/Stage.h>

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_backgroundColor(0.0f, 0.0f, 0.0f)
, m_frame(0)
, m_timeDelta(0.0f)
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

void Stage::invalidateOutput()
{
    outputInvalidated();
}

void Stage::setDeviceViewport(int x, int y, int w, int h)
{
    m_deviceViewport.x = x;
    m_deviceViewport.y = y;
    m_deviceViewport.z = w;
    m_deviceViewport.w = h;
}

void Stage::setVirtualViewport(int x, int y, int w, int h)
{
    m_virtualViewport.x = x;
    m_virtualViewport.y = y;
    m_virtualViewport.z = w;
    m_virtualViewport.w = h;
}

void Stage::setBackgroundColor(float red, float green, float blue)
{
    m_backgroundColor.r = red;
    m_backgroundColor.g = green;
    m_backgroundColor.b = blue;
}

void Stage::setFrameCounter(int frame)
{
    m_frame = frame;
}

void Stage::setTimeDelta(float delta)
{
    m_timeDelta = delta;
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
