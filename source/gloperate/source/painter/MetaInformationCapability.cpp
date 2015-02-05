#include <gloperate/painter/MetaInformationCapability.h>

namespace
{
    const std::string emptyString = "";
}

namespace gloperate
{

/**
*  @brief
*    Constructor
*/
MetaInformationCapability::MetaInformationCapability()
{
}

/**
*  @brief
*    Destructor
*/
MetaInformationCapability::~MetaInformationCapability()
{
}

const std::string & MetaInformationCapability::get(int id) const
{
    if (m_data.count(id) == 0)
    {
        return emptyString;
    }

    return m_data.at(id);
}

void MetaInformationCapability::setMetaInformation(const std::unordered_map<int, std::string> & data)
{
    m_data = data;
}

std::unordered_map<int, std::string> & MetaInformationCapability::data()
{
    return m_data;
}

const std::unordered_map<int, std::string> & MetaInformationCapability::data() const
{
    return m_data;
}

} // namespace gloperate
