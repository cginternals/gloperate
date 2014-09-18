#include <gloperate/pipelines/PropertyGenerator.h>

#include <sstream>
#include <algorithm>

#include <gloperate/pipelines/AbstractConvertibleParameter.h>
#include <gloperate/pipelines/AbstractParameter.h>

namespace {

std::vector<std::string> split(const std::string & s, char delimiter = '/')
{
    std::vector<std::string> elements;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        elements.push_back(item);
    }

    return elements;
}

std::string join(const std::vector<std::string> & strings, char delimiter = '/')
{
    std::stringstream ss;

    for (unsigned i = 0; i < strings.size(); ++i)
    {
        if (i > 0)
            ss << delimiter;

        ss << strings[i];
    }

    return ss.str();
}

}

namespace gloperate {

PropertyGenerator::PropertyGenerator(const std::vector<AbstractParameter*> & parameters, reflectionzeug::PropertyGroup * root)
: m_parameters(parameters)
, m_root(root)
{
}

void PropertyGenerator::generateProperties(const std::vector<AbstractParameter*> & parameters, reflectionzeug::PropertyGroup * root)
{
    PropertyGenerator generator(parameters, root);
    generator.generate();
}

const std::unordered_map<AbstractParameter*, reflectionzeug::AbstractProperty*> & PropertyGenerator::propertyMap()
{
    return m_properties;
}

reflectionzeug::PropertyGroup * PropertyGenerator::obtainGroup(const std::vector<std::string> & path)
{
    if (path.empty())
        return m_root;

    std::string joined = join(path);

    auto it = m_groups.find(joined);
    if (it != m_groups.end())
        return it->second;

    std::vector<std::string> path2 = path;
    std::string groupName = path2.back();
    path2.pop_back();

    std::string name = groupName;
    name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());

    reflectionzeug::PropertyGroup * group = obtainGroup(path2)->addGroup(name);

    group->setOption("title", groupName);

    m_groups[joined] = group;

    return group;
}

void PropertyGenerator::generate()
{
    m_root->clear();
    m_properties.clear();

    for (AbstractParameter * parameter : m_parameters)
    {
        if (!parameter->isConvertible())
            continue;

        auto convertible = dynamic_cast<AbstractConvertibleParameter*>(parameter);

        if (!convertible)
            return;

        auto property = convertible->asProperty();
        m_properties[parameter] = property;

        if (!property)
            continue;

        reflectionzeug::PropertyGroup * group = m_root;

        if (parameter->hasPath())
        {
            std::vector<std::string> path = split(parameter->path());
            while (path.size() > 1 && path.back().empty())
                path.pop_back();

            bool pathContainsName = parameter->path().back() != '/';

            if (pathContainsName)
            {
                property->setOption("title", path.back());
                path.pop_back();
            }

            group = obtainGroup(path);
        }

        group->addProperty(property);
    }
}

} // namespace gloperate
