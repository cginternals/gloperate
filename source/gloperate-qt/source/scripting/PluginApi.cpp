
#include <gloperate-qt/scripting/PluginApi.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>


namespace gloperate_qt {


PluginApi::PluginApi(gloperate::PluginManager * pluginManager)
: reflectionzeug::Object("pluginManager")
, m_pluginManager(pluginManager)
{
    // Register functions
    this->addFunction("paths",      this, &PluginApi::paths);
    this->addFunction("setPaths",   this, &PluginApi::setPaths);
    this->addFunction("addPath",    this, &PluginApi::addPath);
    this->addFunction("removePath", this, &PluginApi::removePath);
    this->addFunction("scan",       this, &PluginApi::scan);
    this->addFunction("load",       this, &PluginApi::load);
    this->addFunction("plugins",    this, &PluginApi::plugins);
    this->addFunction("plugin",     this, &PluginApi::plugin);
}

PluginApi::~PluginApi()
{
}

reflectionzeug::Variant PluginApi::paths()
{
    reflectionzeug::Variant paths = reflectionzeug::Variant::array();

    for (auto path : m_pluginManager->paths()) {
        paths.asArray()->push_back(path);
    }

    return paths;
}

void PluginApi::setPaths(const reflectionzeug::Variant & paths)
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
    m_pluginManager->setPaths(searchPaths);
}

void PluginApi::addPath(const std::string & path)
{
    m_pluginManager->addPath(path);
}

void PluginApi::removePath(const std::string & path)
{
    m_pluginManager->removePath(path);
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
