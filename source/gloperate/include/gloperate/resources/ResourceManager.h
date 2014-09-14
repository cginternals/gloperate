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
class AbstractStorer;


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
        *    Get available storers
        *
        *  @return
        *    List of storers
        */
        const std::vector<AbstractStorer *> & storers() const;

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
        *    Add storer
        *
        *  @param[in] storer
        *    Resource storer
        */
        void addStorer(AbstractStorer * storer);

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

        /**
        *  @brief
        *    Store texture to file
        *
        *  @param filename
        *    File name
        *  @param texture
        *    The texture object
        *
        *  @return
        *    'true', if storage was successful, esle 'false'
        */
        bool storeTexture(const std::string & filename, globjects::Texture * texture) const;


    protected:
        std::vector<AbstractLoader *> m_loaders;    /**< Available loaders */
        std::vector<AbstractStorer *> m_storers;    /**< Available storers */


};


} // namespace gloperate
