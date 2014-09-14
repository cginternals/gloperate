/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/AbstractStorer.h>


namespace globjects {
    class Texture;
}
namespace gloperate
{


/**
*  @brief
*    Texture loader base class
*/
class GLOPERATE_API TextureStorer : public AbstractStorer {


    public:
        /**
        *  @brief
        *    Constructor
        */
        TextureStorer();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~TextureStorer();

        /**
        *  @brief
        *    Store texture to file
        *
        *  @param filename
        *    File name
        *  @param texture
        *   Texture to store
        *
        *  @return
        *   'true' if storage was successful, else 'false'
        */
        bool storeTexture(const std::string & filename, const globjects::Texture * texture) const;


};


} // namespace gloperate
