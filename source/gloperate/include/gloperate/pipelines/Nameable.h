#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

namespace gloperate {
	
class GLOPERATE_API Nameable
{
public:
	Nameable();
	Nameable(const std::string & name);

	virtual ~Nameable();

	const std::string & name() const;
	void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;
protected:
	std::string m_name;
};

} // namespace gloperate
