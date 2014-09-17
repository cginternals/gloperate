/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/resources/ResourceManager.h>
#include <globjects/Texture.h>
#include <gloperate/resources/TextureLoader.h>
#include <gloperate/resources/TextureStorer.h>
 

static std::string getFileExtension(const std::string & filename)
{
    // [TODO] This does not support extensions like ".tar.gz", or files like ".config"

    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos) {
        return filename.substr(pos + 1);
    } else {
        return "";
    }
}


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
*    Load texture from file
*/
globjects::Texture * ResourceManager::loadTexture(const std::string & filename) const
{
    // Get file extension [TODO]
    std::string ext = getFileExtension(filename);

    // Find suitable loader
    for (AbstractLoader * loader : m_loaders) {
        // Check if this is a texture loader
        TextureLoader * textureLoader = dynamic_cast<TextureLoader *>(loader);
        if (textureLoader) {
            // Check if filetype is supported
            if (textureLoader->canLoad(ext)) {
                // Use loader
                return textureLoader->loadTexture(filename);
            }
        }
    }

    // No suitable loader found
    return nullptr;
}

/**
*  @brief
*    Store texture to file
*/
bool ResourceManager::storeTexture(const std::string & filename, globjects::Texture * texture) const
{
    // Get file extension [TODO]
    std::string ext = getFileExtension(filename);

    // Find suitable storer
    for (AbstractStorer * storer : m_storers) {
        // Check if this is a texture loader
        TextureStorer * textureStorer = dynamic_cast<TextureStorer *>(storer);
        if (textureStorer) {
            // Check if filetype is supported
            if (textureStorer->canStore(ext)) {
                // Use store
                return textureStorer->storeTexture(filename, texture);
            }
        }
    }

    // No suitable loader found
    return false;
}


} // namespace gloperate
