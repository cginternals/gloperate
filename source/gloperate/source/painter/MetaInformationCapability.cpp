
#include <gloperate/painter/MetaInformationCapability.h>


namespace
{
    const std::string emptyString = "";
}


namespace gloperate
{


MetaInformationCapability::MetaInformationCapability()
{
}

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

const std::unordered_map<int, std::string> & MetaInformationCapability::data() const
{
    return m_data;
}

std::unordered_map<int, std::string> & MetaInformationCapability::data()
{
    return m_data;
}

void MetaInformationCapability::setMetaInformation(const std::unordered_map<int, std::string> & data)
{
    m_data = data;
}


} // namespace gloperate
