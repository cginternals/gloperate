#include <gloperate/pipelines/StageOwned.h>

#include <gloperate/pipelines/AbstractStage.h>

#include <sstream>
#include <algorithm>

namespace gloperate {

StageOwned::StageOwned(const std::string & name)
: m_owner(nullptr)
, m_name(name)
{
}

StageOwned::~StageOwned()
{
}

const std::string & StageOwned::name() const
{
    return m_name;
}

void StageOwned::setName(const std::string & name)
{
    m_name = name;
}

bool StageOwned::hasName() const
{
    return !m_name.empty();
}

std::string StageOwned::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

bool StageOwned::hasOwner() const
{
    return m_owner != nullptr;
}

const AbstractStage * StageOwned::owner() const
{
    return m_owner;
}

void StageOwned::setOwner(AbstractStage * owner)
{
    m_owner = owner;
}

std::string StageOwned::qualifiedName() const
{
    std::stringstream ss;

    if (hasOwner())
    {
        ss << m_owner->asPrintable() << "::";
    }

    ss << asPrintable();

    return ss.str();
}

} // namespace gloperate
