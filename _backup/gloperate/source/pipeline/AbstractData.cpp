
#include <gloperate/pipeline/AbstractData.h>

#include <algorithm>
#include <sstream>

#include <gloperate/pipeline/AbstractStage.h>


namespace gloperate 
{


AbstractData::AbstractData(const std::string & name)
: m_owner(nullptr)
, m_name(name)
{
}

AbstractData::~AbstractData()
{
}

const std::string & AbstractData::name() const
{
    return m_name;
}

void AbstractData::setName(const std::string & name)
{
    m_name = name;
}

bool AbstractData::hasName() const
{
    return !m_name.empty();
}

std::string AbstractData::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

bool AbstractData::hasOwner() const
{
    return m_owner != nullptr;
}

const AbstractStage * AbstractData::owner() const
{
    return m_owner;
}

void AbstractData::setOwner(AbstractStage * owner)
{
    m_owner = owner;
}

std::string AbstractData::qualifiedName() const
{
    std::stringstream ss;

    if (hasOwner())
    {
        ss << m_owner->asPrintable() << "::";
    }

    ss << asPrintable();

    return ss.str();
}

void AbstractData::invalidate()
{
    invalidated();
}

bool AbstractData::matchesName(const std::string & name) const
{
    return this->name() == name;
}


} // namespace gloperate
