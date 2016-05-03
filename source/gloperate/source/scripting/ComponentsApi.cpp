
#include <gloperate/scripting/ComponentsApi.h>

#include <fstream>

#include <cppexpose/variant/Variant.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


ComponentsApi::ComponentsApi(ViewerContext * viewerContext)
: cppexpose::Object("components")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("pluginPaths",      this, &ComponentsApi::pluginPaths);
    addFunction("addPluginPath",    this, &ComponentsApi::addPluginPath);
    addFunction("removePluginPath", this, &ComponentsApi::removePluginPath);
    addFunction("scanPlugins",      this, &ComponentsApi::scanPlugins);
}

ComponentsApi::~ComponentsApi()
{
}

cppexpose::Variant ComponentsApi::pluginPaths()
{
    std::vector<std::string> paths = m_viewerContext->componentManager()->pluginPaths();

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto path : paths) {
        lst.asArray()->push_back(cppexpose::Variant(path));
    }

    return lst;
}

void ComponentsApi::addPluginPath(const std::string & path)
{
    m_viewerContext->componentManager()->addPluginPath(path);
}

void ComponentsApi::removePluginPath(const std::string & path)
{
    m_viewerContext->componentManager()->removePluginPath(path);
}

void ComponentsApi::scanPlugins(const std::string & identifier)
{
    m_viewerContext->componentManager()->scanPlugins(identifier);
}


} // namespace gloperate
