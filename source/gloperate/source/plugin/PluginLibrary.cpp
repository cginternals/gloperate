
#include <gloperate/plugin/PluginLibrary.h>

#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


PluginLibrary::PluginLibrary(const std::string & filePath)
: m_filePath(filePath)
, m_initPtr(nullptr)
, m_deinitPtr(nullptr)
, m_numPluginsPtr(nullptr)
, m_pluginPtr(nullptr)
{
}

PluginLibrary::~PluginLibrary()
{
}

const std::string & PluginLibrary::filePath() const
{
    return m_filePath;
}

bool PluginLibrary::isValid() const
{
    return (m_initPtr && m_numPluginsPtr && m_pluginPtr && m_deinitPtr);
}

void PluginLibrary::initialize()
{
    if (m_initPtr != nullptr)
    {
        (*m_initPtr)();
    }
}

void PluginLibrary::deinitialize()
{
    if (m_deinitPtr != nullptr)
    {
        (*m_deinitPtr)();
    }
}

unsigned int PluginLibrary::numPlugins() const
{
    if (m_numPluginsPtr != nullptr)
    {
        return (*m_numPluginsPtr)();
    }

    return 0;
}

gloperate::Plugin * PluginLibrary::plugin(unsigned int index) const
{
    if (m_pluginPtr != nullptr)
    {
        return (*m_pluginPtr)(index);
    }

    return nullptr;
}


} // namespace gloperate
