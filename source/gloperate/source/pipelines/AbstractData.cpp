#include <gloperate/pipelines/AbstractData.h>

#include <algorithm>

namespace gloperate {

AbstractData::AbstractData()
{
}

AbstractData::~AbstractData()
{
}

void AbstractData::invalidate()
{
	invalidated();
}

bool AbstractData::matchesName(const std::string & name) const
{
    return this->name() == name || std::find(m_aliasNames.begin(), m_aliasNames.end(), name) != m_aliasNames.end();
}

const std::vector<std::string> & AbstractData::aliasNames() const
{
    return m_aliasNames;
}

void AbstractData::addAlias(const std::string & aliasName)
{
    m_aliasNames.push_back(aliasName);
}

void AbstractData::setAliasNames(const std::vector<std::string> & aliasNames)
{
    m_aliasNames = aliasNames;
}

bool AbstractData::isParameter() const
{
    return false;
}

} // namespace gloperate
