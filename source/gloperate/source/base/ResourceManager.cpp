
#include <gloperate/base/ResourceManager.h>

#include <algorithm>

#include <cppexpose/plugin/ComponentManager.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Loader.h>
#include <gloperate/base/Storer.h>
 

namespace gloperate
{


ResourceManager::ResourceManager(Environment * environment)
: cppexpose::Object("resources")
, m_environment(environment)
{
}

ResourceManager::~ResourceManager()
{
    clearComponents();
}

const std::vector<AbstractLoader *> & ResourceManager::loaders() const
{
    // Return list of loaders
    return m_loaders;
}

const std::vector<AbstractStorer *> & ResourceManager::storers() const
{
    // Return list of storers
    return m_storers;
}

void ResourceManager::updateComponents() const
{
    // Release previous components
    clearComponents();

    // Get available loader components
    auto loaders = m_environment->componentManager()->components<AbstractLoader>();
    for (auto component : loaders) {
        // Create loader
        AbstractLoader * loader = component->createInstance(m_environment);
        m_loaders.push_back(loader);
    }

    // Get available storer components
    auto storers = m_environment->componentManager()->components<AbstractStorer>();
    for (auto component : storers) {
        // Create storer
        AbstractStorer * storer = component->createInstance(m_environment);
        m_storers.push_back(storer);
    }
}

void ResourceManager::clearComponents() const
{
    // Release loaders
    for (AbstractLoader * loader : m_loaders) {
        delete loader;
    }

    // Release storers
    for (AbstractStorer * storer : m_storers) {
        delete storer;
    }
}


} // namespace gloperate
