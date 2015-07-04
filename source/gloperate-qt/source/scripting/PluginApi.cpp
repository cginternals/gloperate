
#include <gloperate-qt/scripting/PluginApi.h>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>


namespace gloperate_qt {


PluginApi::PluginApi(gloperate::PluginManager * pluginManager)
: reflectionzeug::Object("plugin")
, m_pluginManager(pluginManager)
{
    // Register functions
    this->addFunction("list", this, &PluginApi::list);
}

PluginApi::~PluginApi()
{
}

reflectionzeug::Variant PluginApi::list()
{
    reflectionzeug::Variant plugins = reflectionzeug::Variant::array();

    for (auto plugin : m_pluginManager->plugins()) {
        reflectionzeug::Variant info = reflectionzeug::Variant::map();
        (*info.asMap())["Name"]        = plugin->name();
        (*info.asMap())["Type"]        = plugin->type();
        (*info.asMap())["Version"]     = plugin->version();
        (*info.asMap())["Vendor"]      = plugin->vendor();
        (*info.asMap())["Description"] = plugin->description();

        plugins.asArray()->push_back(info);
    }

    return plugins;
}


} // namespace gloperate_qt
