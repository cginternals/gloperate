#include <gloperate/pipelines/StageOwned.h>

#include <gloperate/pipelines/AbstractStage.h>

#include <sstream>

namespace gloperate {

StageOwned::StageOwned()
: m_owner(nullptr)
{
}

StageOwned::~StageOwned()
{
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
