#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


PluginLibrary::PluginLibrary(const std::string & filename)
: m_filename(filename)
, m_initPluginPtr(nullptr)
, m_getNumOfPluginsPtr(nullptr)
, m_getPluginPtr(nullptr)
, m_deinitPluginPtr(nullptr)
{
}

PluginLibrary::~PluginLibrary()
{
}

const std::string & PluginLibrary::filename() const
{
    return m_filename;
}

bool PluginLibrary::isValid() const
{
    return (m_initPluginPtr      != nullptr &&
            m_getNumOfPluginsPtr != nullptr &&
            m_getPluginPtr       != nullptr &&
            m_deinitPluginPtr    != nullptr );
}

void PluginLibrary::initialize()
{
    // Call plugin function
    if (m_initPluginPtr != nullptr) {
        (*m_initPluginPtr)();
    }
}

unsigned int PluginLibrary::getNumOfPlugins() const
{
    // Call plugin function
    if (m_getNumOfPluginsPtr != nullptr) {
        return (*m_getNumOfPluginsPtr)();
    } else {
        return 0;
    }
}

gloperate::Plugin * PluginLibrary::getPlugin(unsigned int index) const
{
    // Call plugin function
    if (m_getPluginPtr != nullptr) {
        return (*m_getPluginPtr)(index);
    } else {
        return nullptr;
    }
}

void PluginLibrary::deinitialize()
{
    // Call plugin function
    if (m_deinitPluginPtr != nullptr) {
        (*m_deinitPluginPtr)();
    }
}


} // namespace gloperate
