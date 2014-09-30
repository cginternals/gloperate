#pragma once

#include <string>

#include <gloperate/signals/Signal.h>

#include <gloperate/gloperate_api.h>

namespace gloperate {

class AbstractStage;

class GLOPERATE_API AbstractData
{
    friend class AbstractStage;
public:
    AbstractData(const std::string & name = "");
	virtual ~AbstractData();

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

    bool hasOwner() const;
    const AbstractStage * owner() const;

    virtual std::string qualifiedName() const;

	void invalidate();

    bool matchesName(const std::string & name) const;

    virtual std::string type() const = 0;
public:
    AbstractStage * m_owner;
    std::string m_name;

    gloperate::Signal<> invalidated;

    void setOwner(AbstractStage * owner);
};

} // namespace gloperate
