#include <gloperate/pipelines/AbstractParameter.h>
	
namespace gloperate {
	
AbstractParameter::AbstractParameter()
{
}

AbstractParameter::~AbstractParameter()
{
}

const std::string & AbstractParameter::path() const
{
    return m_path;
}

void AbstractParameter::setPath(const std::string & path)
{
    m_path = path;
}

bool AbstractParameter::hasPath() const
{
    return !m_path.empty();
}

bool AbstractParameter::isParameter() const
{
    return true;
}

bool AbstractParameter::isConvertible() const
{
    return false;
}

} // namespace gloperate
