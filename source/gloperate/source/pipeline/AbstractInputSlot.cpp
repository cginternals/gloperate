
#include <gloperate/pipeline/AbstractInputSlot.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractInputSlot::AbstractInputSlot()
: m_owner(nullptr)
, m_feedback(false)
{
}

AbstractInputSlot::~AbstractInputSlot()
{
    if (m_owner) {
        m_owner->unregisterInput(this);
    }
}

Stage * AbstractInputSlot::owner() const
{
    return m_owner;
}

std::string AbstractInputSlot::qualifiedName() const
{
    std::stringstream ss;

    if (m_owner)
    {
        ss << m_owner->name() << ".";
    }

    ss << name();

    return ss.str();
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

void AbstractInputSlot::initInputSlot(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerInput(this);
    }
}


} // namespace gloperate
