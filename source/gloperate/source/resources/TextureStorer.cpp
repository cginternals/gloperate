/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <globjects/Texture.h>
#include <gloperate/resources/TextureStorer.h>
 

namespace gloperate
{


/**
*  @brief
*    Constructor
*/
TextureStorer::TextureStorer()
: AbstractStorer()
{
}

/**
*  @brief
*    Destructor
*/
TextureStorer::~TextureStorer()
{
}

/**
*  @brief
*    Store texture to file
*/
bool TextureStorer::storeTexture(const std::string & filename, const globjects::Texture * texture) const
{
    return this->store(filename, texture);
}


} // namespace gloperate
