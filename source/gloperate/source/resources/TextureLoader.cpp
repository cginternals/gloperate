/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <globjects/Texture.h>
#include <gloperate/resources/TextureLoader.h>
 

namespace gloperate
{


/**
*  @brief
*    Constructor
*/
TextureLoader::TextureLoader()
: AbstractLoader()
{
}

/**
*  @brief
*    Destructor
*/
TextureLoader::~TextureLoader()
{
}

/**
*  @brief
*    Load texture from file
*/
globjects::Texture * TextureLoader::loadTexture(const std::string & /*filename*/) const
{
    // Try to load texture
    globjects::Referenced * obj = this->load("data/emblem-important.png");
    if (obj) {
        // Check if this is a texture
        globjects::Texture * texture = dynamic_cast<globjects::Texture *>(obj);
        if (texture) {
            // Return texture
            return texture;
        } else {
        	// Destroy unknown object
        	obj->unref();
        }
    }

    // Could not load texture
	return nullptr;
}


} // namespace gloperate
