/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <string>
#include <vector>
#include <gloperate/gloperate_api.h>


namespace globjects {
    class Texture;
}
namespace gloperate
{


class AbstractLoader;


/**
*  @brief
*    Class to help loading/accessing resources (textures, ...)
*/
class GLOPERATE_API ResourceManager {


    public:
        /**
        *  @brief
        *    Constructor
        */
        ResourceManager();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~ResourceManager();

        /**
        *  @brief
        *    Get available loaders
        *
        *  @return
        *    List of loaders
        */
        const std::vector<AbstractLoader *> & loaders() const;

        /**
        *  @brief
        *    Add loader
        *
        *  @param[in] loader
        *    Resource loader
        */
        void addLoader(AbstractLoader * loader);

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


    protected:
        std::vector<AbstractLoader *> m_loaders;    /**< Available loaders */


};


} // namespace gloperate
