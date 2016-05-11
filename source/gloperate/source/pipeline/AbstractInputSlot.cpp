
#include <gloperate/pipeline/AbstractInputSlot.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractInputSlot::AbstractInputSlot(Stage * parent)
: m_feedback(false)
{
    if (parent) {
        parent->registerInput(this);
    }
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
