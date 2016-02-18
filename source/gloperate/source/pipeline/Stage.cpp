
#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
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

const GLContextFormat & Stage::requiredFormat() const
{
    static GLContextFormat format;

    format.setVersion(3, 3);
    return format;
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
