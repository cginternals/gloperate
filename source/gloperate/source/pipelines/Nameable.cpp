#include <gloperate/pipelines/Nameable.h>

#include <algorithm>

namespace gloperate {
	
Nameable::Nameable()
{
}
	
Nameable::~Nameable()
{
}

Nameable::Nameable(const std::string & name)
: m_name(name)
{
}

const std::string & Nameable::name() const
{
	return m_name;
}

void Nameable::setName(const std::string & name)
{
	m_name = name;
}

bool Nameable::hasName() const
{
    return !m_name.empty();
}

std::string Nameable::asPrintable() const
{
    if (!hasName())
        return "<unnamed>";

    std::string n = name();
    std::replace(n.begin(), n.end(), ' ', '_');

    return n;
}

} // namespace gloperate
