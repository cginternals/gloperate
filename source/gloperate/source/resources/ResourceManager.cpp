
#include <gloperate/resources/ResourceManager.h>

#include <algorithm>

#include <gloperate/resources/Loader.h>
#include <gloperate/resources/Storer.h>
 

namespace gloperate
{


/**
*  @brief
*    Constructor
*/
ResourceManager::ResourceManager()
{
}

/**
*  @brief
*    Destructor
*/
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

/**
*  @brief
*    Get available loaders
*/
const std::vector<AbstractLoader *> & ResourceManager::loaders() const
{
    // Return list of loaders
    return m_loaders;
}

/**
*  @brief
*    Get available storers
*/
const std::vector<AbstractStorer *> & ResourceManager::storers() const
{
    // Return list of storers
    return m_storers;
}

/**
*  @brief
*    Add loader
*/
void ResourceManager::addLoader(AbstractLoader * loader)
{
    // Add loader to list
    m_loaders.push_back(loader);
}

/**
*  @brief
*    Add loader
*/
void ResourceManager::addStorer(AbstractStorer * storer)
{
    // Add storer to list
    m_storers.push_back(storer);
}

/**
*  @brief
*    Get file extension
*/
std::string ResourceManager::getFileExtension(const std::string & filename) const
{
    // [TODO] This does not support extensions like ".tar.gz", or files like ".config"

    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos) {
        std::string ext = filename.substr(pos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
        return ext;
    } else {
        return "";
    }
}


} // namespace gloperate
