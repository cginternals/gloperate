#include <gloperate/resources/ResourceManager.h>
#include <gloperate/resources/Loader.h>
#include <gloperate/resources/Storer.h>
 

namespace gloperate
{


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
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

void ResourceManager::addLoader(AbstractLoader * loader)
{
    // Add loader to list
    m_loaders.push_back(loader);
}

void ResourceManager::addStorer(AbstractStorer * storer)
{
    // Add storer to list
    m_storers.push_back(storer);
}

std::string ResourceManager::getFileExtension(const std::string & filename) const
{
    // [TODO] This does not support extensions like ".tar.gz", or files like ".config"

    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos) {
        return filename.substr(pos + 1);
    } else {
        return "";
    }
}


} // namespace gloperate
