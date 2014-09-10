/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/AbstractLoader.h>


namespace globjects {
    class Texture;
}
namespace gloperate
{


/**
*  @brief
*    Texture loader base class
*/
class GLOPERATE_API TextureLoader : public AbstractLoader {


    public:
        /**
        *  @brief
        *    Constructor
        */
        TextureLoader();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~TextureLoader();

        /**
        *  @brief
        *    Load texture from file
        *
        *  @param filename
        *    File name
        *
        *  @return
        *    Loaded texture (can be null)
        */
        globjects::Texture * loadTexture(const std::string & filename) const;


};


} // namespace gloperate
