
#include <gloperate/stages/demos/DemoTimerStage.h>

#include <cppassist/logging/logging.h>


using namespace cppassist;


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoTimerStage, gloperate::Stage)


DemoTimerStage::DemoTimerStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, timeDelta  ("timeDelta",   this, 0.0f)
, virtualTime("virtualTime", this, 0.0f)
, m_timer(environment)
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


} // namespace gloperate
