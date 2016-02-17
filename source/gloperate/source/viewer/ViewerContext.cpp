
#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


ViewerContext::ViewerContext()
: m_timeManager(this)
{
}

ViewerContext::~ViewerContext()
{
}

const TimeManager * ViewerContext::timeManager() const
{
    return &m_timeManager;
}

TimeManager * ViewerContext::timeManager()
{
    return &m_timeManager;
}


} // namespace gloperate
