
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

std::vector<AbstractLoader *> ResourceManager::loaders() const
{
    // Get list of raw pointers
    std::vector<AbstractLoader *> loaders;
    std::transform(m_loaders.begin(), m_loaders.end(), std::back_inserter(loaders), [] (const std::unique_ptr<AbstractLoader> & loader)
    {
        return loader.get();
    } );

    // Return list of loaders
    return loaders;
}

std::vector<AbstractStorer *> ResourceManager::storers() const
{
    // Get list of raw pointers
    std::vector<AbstractStorer *> storers;
    std::transform(m_storers.begin(), m_storers.end(), std::back_inserter(storers), [](const std::unique_ptr<AbstractStorer> & storer) { return storer.get(); });

    // Return list of storers
    return storers;
}

void ResourceManager::updateComponents() const
{
    // Release previous components
    clearComponents();

    // Get available loader components
    auto loaders = m_environment->componentManager()->components<AbstractLoader>();
    for (auto component : loaders) {
        // Create loader
        auto loader = component->createInstance(m_environment);
        m_loaders.push_back(std::move(loader));
    }

    // Get available storer components
    auto storers = m_environment->componentManager()->components<AbstractStorer>();
    for (auto component : storers) {
        // Create storer
        auto storer = component->createInstance(m_environment);
        m_storers.push_back(std::move(storer));
    }
}

void ResourceManager::clearComponents() const
{
    m_loaders.clear();
    m_storers.clear();
}


} // namespace gloperate
