#include <gloperate/pipelines/AbstractInputSlot.h>
	
namespace gloperate {
	
AbstractInputSlot::AbstractInputSlot()
: m_hasChanged(true)
, m_isOptional(false)
, m_isFeedback(false)
{
}

AbstractInputSlot::~AbstractInputSlot()
{
}

bool AbstractInputSlot::isUsable() const
{
    return isOptional() || isConnected();
}

bool AbstractInputSlot::hasChanged() const
{
    return m_hasChanged;
}

void AbstractInputSlot::changed()
{
    m_hasChanged = true;
}

void AbstractInputSlot::processed()
{
    m_hasChanged = false;
}

bool AbstractInputSlot::isOptional() const
{
    return m_isOptional;
}

void AbstractInputSlot::setOptional(bool optional)
{
    m_isOptional = optional;
}

bool AbstractInputSlot::isFeedback() const
{
    return m_isFeedback;
}

void AbstractInputSlot::setFeedback(bool isFeedback)
{
    m_isFeedback = isFeedback;

    connectionChanged();
}

bool AbstractInputSlot::isConnected() const
{
    return connectedData() != nullptr;
}

} // namespace gloperate
