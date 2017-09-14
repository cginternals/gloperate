
#include <gloperate/base/ComponentManager.h>

#include <fstream>

#include <cppexpose/reflection/Variant.h>


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
    std::vector<std::string> paths = cppassist::string::split(allPaths, ';');

    for (auto path : paths) {
        this->addPluginPath(path, PluginPathType::UserDefined);
    }
}

cppexpose::Variant ComponentManager::scr_pluginPaths()
{
    std::vector<std::string> paths = this->pluginPaths(PluginPathType::UserDefined);

    cppexpose::Variant lst = cppexpose::Variant::array();
    for (auto path : paths) {
        lst.pushElement(cppexpose::Variant(path));
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

        obj.setElement("name", cppexpose::Variant(component->name()));
        obj.setElement("description", cppexpose::Variant(component->description()));
        obj.setElement("type", cppexpose::Variant(component->type()));
        obj.setElement("tags", cppexpose::Variant(component->tags()));
        obj.setElement("icon", cppexpose::Variant(component->icon()));
        obj.setElement("annotations", cppexpose::Variant(component->annotations()));
        obj.setElement("vendor", cppexpose::Variant(component->vendor()));
        obj.setElement("version", cppexpose::Variant(component->version()));

        lst.pushElement(obj);
    }

    return lst;
}

void ComponentManager::scr_printComponents()
{
    this->printComponents();
}


} // namespace gloperate
