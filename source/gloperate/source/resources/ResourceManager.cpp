/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/resources/ResourceManager.h>
#include <globjects/Texture.h>
#include <gloperate/resources/TextureLoader.h>
 

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
*    Add loader
*/
void ResourceManager::addLoader(AbstractLoader * loader)
{
    // Add loader to list
    m_loaders.push_back(loader);
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


} // namespace gloperate
