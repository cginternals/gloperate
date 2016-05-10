
#include <gloperate/pipeline/AbstractInputSlot.h>


namespace gloperate
{


AbstractInputSlot::AbstractInputSlot()
: m_feedback(false)
{
}

AbstractInputSlot::~AbstractInputSlot()
{
}

bool AbstractInputSlot::isConnected() const
{
    return source() != nullptr;
}

bool AbstractInputSlot::isFeedback() const
{
    return m_feedback;
}

void AbstractInputSlot::setFeedback(bool feedback)
{
    m_feedback = feedback;
}


} // namespace gloperate
