#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include <reflectionzeug/PropertyGroup.h>

#include <gloperate/gloperate_api.h>

namespace reflectionzeug {
    class AbstractProperty;
}

namespace gloperate {

class AbstractParameter;

class GLOPERATE_API PropertyGenerator
{
public:
    PropertyGenerator(const std::vector<AbstractParameter*> & parameters, reflectionzeug::PropertyGroup * root);

    void generate();
    const std::unordered_map<AbstractParameter *, reflectionzeug::AbstractProperty *> & propertyMap();

    static void generateProperties(const std::vector<AbstractParameter*> & parameters, reflectionzeug::PropertyGroup * root);
protected:
    const std::vector<AbstractParameter*> & m_parameters;
    reflectionzeug::PropertyGroup * m_root;
    std::unordered_map<std::string, reflectionzeug::PropertyGroup*> m_groups;
    std::unordered_map<AbstractParameter*, reflectionzeug::AbstractProperty*> m_properties;

    reflectionzeug::PropertyGroup * obtainGroup(const std::vector<std::string> & path);
};

} // namespace gloperate
