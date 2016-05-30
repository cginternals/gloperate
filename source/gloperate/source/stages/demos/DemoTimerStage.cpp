
#include <gloperate/stages/demos/DemoTimerStage.h>

#include <cppassist/logging/logging.h>

#include <gloperate/gloperate-version.h>


using namespace cppassist;


namespace gloperate
{


DemoTimerStage::DemoTimerStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
, timeDelta  ("timeDelta",   this, 0.0f)
, virtualTime("virtualTime", this, 0.0f)
, m_timer(viewerContext)
, m_time(0.0f)
{
    // Setup timer
    m_timer.elapsed.connect([this] ()
    {
        // Update virtual time
        m_time += *timeDelta;
        virtualTime.setValue(m_time);
    });

    m_timer.start(0.0f);
}

DemoTimerStage::~DemoTimerStage()
{
}

void DemoTimerStage::onContextInit(AbstractGLContext *)
{
}

void DemoTimerStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoTimerStage::onProcess(AbstractGLContext *)
{
}


CPPEXPOSE_COMPONENT(
    DemoTimerStage, gloperate::Stage
  , ""   // Tags
  , ""   // Icon
  , ""   // Annotations
  , "Demo stage that creates a constant timer"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate
