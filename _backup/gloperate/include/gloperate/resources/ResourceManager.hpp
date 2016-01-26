
#pragma once


#include <gloperate/resources/ResourceManager.h>

#include <gloperate/resources/Loader.h>
#include <gloperate/resources/Storer.h>


namespace gloperate
{


template <typename T>
T * ResourceManager::load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const
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
                return concreteLoader->load(filename, options, progress);
            }
        }
    }

    // No suitable loader found
    return nullptr;
}

template <typename T>
bool ResourceManager::store(const std::string & filename, T * resource, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const
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
                return concreteStorer->store(filename, resource, options, progress);
            }
        }
    }

    // No suitable loader found
    return false;
}


} // namespace gloperate
