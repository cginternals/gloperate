
#include <gloperate-qt/scripting/PluginApi.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>


namespace gloperate_qt {


PluginApi::PluginApi(gloperate::PluginManager * pluginManager)
: reflectionzeug::Object("pluginManager")
, m_pluginManager(pluginManager)
{
    // Register functions
    this->addFunction("searchPaths",      this, &PluginApi::searchPaths);
    this->addFunction("setSearchPaths",   this, &PluginApi::setSearchPaths);
    this->addFunction("addSearchPath",    this, &PluginApi::addSearchPath);
    this->addFunction("removeSearchPath", this, &PluginApi::removeSearchPath);
    this->addFunction("scan",             this, &PluginApi::scan);
    this->addFunction("load",             this, &PluginApi::load);
    this->addFunction("plugins",          this, &PluginApi::plugins);
    this->addFunction("plugin",           this, &PluginApi::plugin);
}

PluginApi::~PluginApi()
{
}

reflectionzeug::Variant PluginApi::searchPaths()
{
    reflectionzeug::Variant paths = reflectionzeug::Variant::array();

    for (auto path : m_pluginManager->searchPaths()) {
        paths.asArray()->push_back(path);
    }

    return paths;
}

void PluginApi::setSearchPaths(const reflectionzeug::Variant & paths)
{
    // Argument must be an array
    if (!paths.isArray()) {
        return;
    }

    // Get search paths
    std::vector<std::string> searchPaths;
    for (const reflectionzeug::Variant & path : *paths.asArray()) {
        searchPaths.push_back(path.value<std::string>());
    }

    // Set new search paths
    m_pluginManager->setSearchPaths(searchPaths);
}

void PluginApi::addSearchPath(const std::string & path)
{
    m_pluginManager->addSearchPath(path);
}

void PluginApi::removeSearchPath(const std::string & path)
{
    m_pluginManager->removeSearchPath(path);
}

void PluginApi::scan(const std::string & identifier, bool reload)
{
    m_pluginManager->scan(identifier, reload);
}

bool PluginApi::load(const std::string & filePath, bool reload)
{
    return m_pluginManager->load(filePath, reload);
}

reflectionzeug::Variant PluginApi::plugins()
{
    reflectionzeug::Variant plugins = reflectionzeug::Variant::array();
    for (auto plugin : m_pluginManager->plugins()) {
        plugins.asArray()->push_back(getPluginInfo(plugin));
    }

    return plugins;
}

reflectionzeug::Variant PluginApi::plugin(const std::string & name)
{
    // Get plugin
    gloperate::Plugin * plugin = m_pluginManager->plugin(name);

    // Return plugin info
    return getPluginInfo(plugin);
}

reflectionzeug::Variant PluginApi::getPluginInfo(gloperate::Plugin * plugin) const
{
    // Check if plugin is valid
    if (!plugin) {
        return reflectionzeug::Variant();
    }

    // Get plugin info
    reflectionzeug::Variant info = reflectionzeug::Variant::map();
    (*info.asMap())["Name"]        = plugin->name();
    (*info.asMap())["Type"]        = plugin->type();
    (*info.asMap())["Version"]     = plugin->version();
    (*info.asMap())["Vendor"]      = plugin->vendor();
    (*info.asMap())["Description"] = plugin->description();
    return info;
}


} // namespace gloperate_qt
