
#pragma once


#include <gloperate/resources/ResourceManager.h>
#include <gloperate/resources/Loader.h>
#include <gloperate/resources/Storer.h>


namespace gloperate
{


/**
*  @brief
*    Load resource from file
*/
template <typename T>
T * ResourceManager::load(const std::string & filename, std::function<void(int, int)> progress) const
{
    // Get file extension
    std::string ext = getFileExtension(filename);

    // Find suitable loader
    for (AbstractLoader * loader : m_loaders) {
        // Check loader type
        Loader<T> * concreteLoader = dynamic_cast<Loader<T> *>(loader);
        if (concreteLoader) {
            // Check if filetype is supported
            if (concreteLoader->canLoad(ext)) {
                // Use loader
                return concreteLoader->load(filename, progress);
            }
        }
    }

    // No suitable loader found
    return nullptr;
}

/**
*  @brief
*    Store resource to file
*/
template <typename T>
bool ResourceManager::store(const std::string & filename, T * resource, std::function<void(int, int)> progress) const
{
    // Get file extension
    std::string ext = getFileExtension(filename);

    // Find suitable storer
    for (AbstractStorer * storer : m_storers) {
        // Check storer type
        Storer<T> * concreteStorer = dynamic_cast<Storer<T> *>(storer);
        if (concreteStorer) {
            // Check if filetype is supported
            if (concreteStorer->canStore(ext)) {
                // Use store
                return concreteStorer->store(filename, resource, progress);
            }
        }
    }

    // No suitable loader found
    return false;
}


} // namespace gloperate
