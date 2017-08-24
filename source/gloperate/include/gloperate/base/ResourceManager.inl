
#pragma once


#include <cppassist/fs/FilePath.h>

#include <gloperate/base/Loader.h>
#include <gloperate/base/Storer.h>


using namespace cppassist;


namespace gloperate
{


template <typename T>
T * ResourceManager::load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const
{
    // Lazy initialization of loaders
    if (m_loaders.size() == 0) {
        updateComponents();
    }

    // Get file extension
    std::string ext = FilePath(filename).extension();
    auto pos = ext.find_last_of('.');
    if (pos != std::string::npos)
    {
        ext = ext.substr(pos + 1);
    }

    // Find suitable loader
    for (const auto & loader : m_loaders) {
        // Check loader type
        Loader<T> * concreteLoader = dynamic_cast<Loader<T> *>(loader.get());
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
bool ResourceManager::store(const std::string & filename, T * resource, const cppexpose::Variant & options, std::function<void(int, int)> progress) const
{
    // Lazy initialization of storers
    if (m_storers.size() == 0) {
        updateComponents();
    }

    // Get file extension
    std::string ext = FilePath(filename).extension();

    // Find suitable storer
    for (const auto & storer : m_storers) {
        // Check storer type
        Storer<T> * concreteStorer = dynamic_cast<Storer<T> *>(storer.get());
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
