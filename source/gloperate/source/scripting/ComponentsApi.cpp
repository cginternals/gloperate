
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
    addFunction("components",       this, &ComponentsApi::components);
    addFunction("printComponents",  this, &ComponentsApi::printComponents);
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

cppexpose::Variant ComponentsApi::components()
{
    cppexpose::Variant lst = cppexpose::Variant::array();

    auto & components = m_viewerContext->componentManager()->components();
    for (auto * component : components) {
        cppexpose::Variant obj = cppexpose::Variant::map();
        cppexpose::VariantMap & map = *obj.asMap();

        map["name"]        = cppexpose::Variant(component->name());
        map["description"] = cppexpose::Variant(component->description());
        map["type"]        = cppexpose::Variant(component->type());
        map["tags"]        = cppexpose::Variant(component->tags());
        map["icon"]        = cppexpose::Variant(component->icon());
        map["annotations"] = cppexpose::Variant(component->annotations());
        map["vendor"]      = cppexpose::Variant(component->vendor());
        map["version"]     = cppexpose::Variant(component->version());

        lst.asArray()->push_back(obj);
    }

    return lst;
}

void ComponentsApi::printComponents()
{
    m_viewerContext->componentManager()->printComponents();
}


} // namespace gloperate
