
#include <gloperate/pipeline/AbstractInputSlot.h>

#include <sstream>
#include <algorithm>

#include <gloperate/pipeline/AbstractStage.h>


namespace gloperate 
{


AbstractInputSlot::AbstractInputSlot(const std::string & name)
: m_owner(nullptr)
, m_name(name)
, m_hasChanged(true)
, m_isOptional(false)
, m_isFeedback(false)
{
}

AbstractInputSlot::~AbstractInputSlot()
{
}

const std::string & AbstractInputSlot::name() const
{
    return m_name;
}

void AbstractInputSlot::setName(const std::string & name)
{
    m_name = name;
}

bool AbstractInputSlot::hasName() const
{
    return !m_name.empty();
}

std::string AbstractInputSlot::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

bool AbstractInputSlot::hasOwner() const
{
    return m_owner != nullptr;
}

const AbstractStage * AbstractInputSlot::owner() const
{
    return m_owner;
}

void AbstractInputSlot::setOwner(AbstractStage * owner)
{
    m_owner = owner;
}

std::string AbstractInputSlot::qualifiedName() const
{
    std::stringstream ss;

    if (hasOwner())
    {
        ss << m_owner->asPrintable() << "::";
    }

    ss << asPrintable();

    return ss.str();
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
