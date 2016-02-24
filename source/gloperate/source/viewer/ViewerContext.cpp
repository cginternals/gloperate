
#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


ViewerContext::ViewerContext()
: m_timeManager(this)
, m_inputManager()
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

const InputManager *ViewerContext::inputManager() const
{
    return &m_inputManager;
}

InputManager *ViewerContext::inputManager()
{
    return &m_inputManager;

}


} // namespace gloperate
