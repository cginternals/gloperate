
#include <gloperate/pipeline/AbstractInputSlot.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractInputSlot::AbstractInputSlot(SlotType type)
: AbstractSlot(type)
, m_feedback(false)
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
