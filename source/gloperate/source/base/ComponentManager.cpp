
#include <gloperate/base/ComponentManager.h>

#include <fstream>

#include <cppexpose/variant/Variant.h>


using namespace cppexpose;


namespace gloperate
{


ComponentManager::ComponentManager()
: cppexpose::Object("components")
{
    // Register functions
    addFunction("getPluginPaths",   this, &ComponentManager::scr_getPluginPaths);
    addFunction("setPluginPaths",   this, &ComponentManager::scr_setPluginPaths);
    addFunction("pluginPaths",      this, &ComponentManager::scr_pluginPaths);
    addFunction("addPluginPath",    this, &ComponentManager::scr_addPluginPath);
    addFunction("removePluginPath", this, &ComponentManager::scr_removePluginPath);
    addFunction("scanPlugins",      this, &ComponentManager::scr_scanPlugins);
    addFunction("components",       this, &ComponentManager::scr_components);
    addFunction("printComponents",  this, &ComponentManager::scr_printComponents);
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::scanPlugins()
{
    #ifndef NDEBUG
        cppexpose::ComponentManager::scanPlugins("-plugins-debug");
    #else
        cppexpose::ComponentManager::scanPlugins("-plugins");
    #endif
}

std::string ComponentManager::scr_getPluginPaths()
{
    std::vector<std::string> paths = this->pluginPaths(PluginPathType::UserDefined);

    std::string allPaths = "";
    for (auto path : paths) {
        if (!allPaths.empty()) allPaths += ";";
        allPaths += path;
    }

    return allPaths;
}

void ComponentManager::scr_setPluginPaths(const std::string & allPaths)
{
    std::vector<std::string> paths = cppassist::string::split(allPaths, ';', false);

    for (auto path : paths) {
        this->addPluginPath(path, PluginPathType::UserDefined);
    }
}

cppexpose::Variant ComponentManager::scr_pluginPaths()
{
    std::vector<std::string> paths = this->pluginPaths(PluginPathType::UserDefined);

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto path : paths) {
        lst.asArray()->push_back(cppexpose::Variant(path));
    }

    return lst;
}

void ComponentManager::scr_addPluginPath(const std::string & path)
{
    this->addPluginPath(path, PluginPathType::UserDefined);
}

void ComponentManager::scr_removePluginPath(const std::string & path)
{
    this->removePluginPath(path);
}

void ComponentManager::scr_scanPlugins()
{
    scanPlugins();
}

cppexpose::Variant ComponentManager::scr_components()
{
    cppexpose::Variant lst = cppexpose::Variant::array();

    auto & components = this->components();
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

void ComponentManager::scr_printComponents()
{
    this->printComponents();
}


} // namespace gloperate
