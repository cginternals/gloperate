
#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


Plugin::Plugin(
    const std::string & type
  , const std::string & name
  , const std::string & description
  , const std::string & vendor
  , const std::string & version)
: m_type(type)
, m_name(name)
, m_description(description)
, m_vendor(vendor)
, m_version(version)
{
}

Plugin::~Plugin()
{
}

const char * Plugin::type() const
{
    return m_type.c_str();
}

const char * Plugin::name() const
{
    return m_name.c_str();
}

const char * Plugin::description() const
{
    return m_description.c_str();
}

const char * Plugin::vendor() const
{
    return m_vendor.c_str();
}

const char * Plugin::version() const
{
    return m_version.c_str();
}

const cpplocate::ModuleInfo & Plugin::moduleInfo() const
{
    return m_moduleInfo;
}

void Plugin::setModuleInfo(const cpplocate::ModuleInfo & moduleInfo)
{
    m_moduleInfo = moduleInfo;
}


} // namespace gloperate
