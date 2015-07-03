
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
        plugins.asArray()->push_back(plugin->name());
    }

    return plugins;
}


} // namespace gloperate_qt
