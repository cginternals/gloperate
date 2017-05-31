
#include "TimerStage.h"

#include <cppassist/logging/logging.h>


using namespace cppassist;


CPPEXPOSE_COMPONENT(TimerStage, gloperate::Stage)


TimerStage::TimerStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TimerStage", name)
, timeDelta  ("timeDelta",   this, 0.0f)
, factor     ("factor",      this, 1.0f)
, virtualTime("virtualTime", this, 0.0f)
, m_time(0.0f)
{
}

TimerStage::~TimerStage()
{
}

void TimerStage::onContextInit(gloperate::AbstractGLContext *)
{
}

void TimerStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void TimerStage::onProcess()
{
    m_time += (*timeDelta) * (*factor);

    virtualTime.setValue(m_time);
}
