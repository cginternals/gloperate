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
class ResourceManager {


    public:
        /**
        *  @brief
        *    Constructor
        */
        GLOPERATE_API ResourceManager();

        /**
        *  @brief
        *    Destructor
        */
        GLOPERATE_API virtual ~ResourceManager();

        /**
        *  @brief
        *    Get available loaders
        *
        *  @return
        *    List of loaders
        */
        GLOPERATE_API const std::vector<AbstractLoader *> & loaders() const;

        /**
        *  @brief
        *    Get available storers
        *
        *  @return
        *    List of storers
        */
        GLOPERATE_API const std::vector<AbstractStorer *> & storers() const;

        /**
        *  @brief
        *    Add loader
        *
        *  @param[in] loader
        *    Resource loader
        */
        GLOPERATE_API void addLoader(AbstractLoader * loader);

        /**
        *  @brief
        *    Add storer
        *
        *  @param[in] storer
        *    Resource storer
        */
        GLOPERATE_API void addStorer(AbstractStorer * storer);

        /**
        *  @brief
        *    Load resource from file
        *
        *  @param filename
        *    File name
        *
        *  @return
        *    Loaded resource (can be null)
        */
        template <typename T>
        T * load(const std::string & filename) const;

        /**
        *  @brief
        *    Store resource to file
        *
        *  @param filename
        *    File name
        *  @param resource
        *    The resource object
        *
        *  @return
        *    'true', if storage was successful, esle 'false'
        */
        template <typename T>
        bool store(const std::string & filename, T * resource) const;


    protected:
        /**
        *  @brief
        *    Get file extension
        *
        *  @param[in] filename
        *    Path to file (with filename and extension)
        *
        *  @return
        *    Fielname extension
        */
        GLOPERATE_API std::string getFileExtension(const std::string & filename) const;


    protected:
        std::vector<AbstractLoader *> m_loaders;    /**< Available loaders */
        std::vector<AbstractStorer *> m_storers;    /**< Available storers */


};


} // namespace gloperate


#include <gloperate/resources/ResourceManager.hpp>
