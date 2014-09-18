#pragma once

#include <vector>

#include <gloperate/signals/Signal.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/StageOwned.h>

namespace gloperate {
	
class GLOPERATE_API AbstractData : public StageOwned
{
public:
	AbstractData();
	virtual ~AbstractData();

	void invalidate();

    bool matchesName(const std::string & name) const;

    const std::vector<std::string> & aliasNames() const;
    void addAlias(const std::string & aliasName);
    void setAliasNames(const std::vector<std::string> & aliasNames);

    virtual bool isParameter() const;

    virtual std::string type() const = 0;
public:
	gloperate::Signal<> invalidated;
    std::vector<std::string> m_aliasNames;
};

} // namespace gloperate
