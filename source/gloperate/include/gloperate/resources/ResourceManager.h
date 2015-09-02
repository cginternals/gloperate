
#pragma once


#include <string>
#include <vector>
#include <functional>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
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
class GLOPERATE_API ResourceManager
{
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
    *    Load resource from file
    *
    *  @param[in] filename
    *    File name
    *  @param[in] options
    *    Options for loading resource, see documentation of specific loader for supported options
    *  @param[in] progress
    *    Callback function that is invoked on progress (can be empty)
    *
    *  @return
    *    Loaded resource (can be null)
    */
    template <typename T>
    T * load(const std::string & filename, const reflectionzeug::Variant & options = reflectionzeug::Variant(), std::function<void(int, int)> progress = std::function<void(int, int)>()) const;

    /**
    *  @brief
    *    Store resource to file
    *
    *  @param[in] filename
    *    File name
    *  @param[in] resource
    *    The resource object
    *  @param[in] options
    *    Options for loading resource, see documentation of specific storer for supported options
    *  @param[in] progress
    *    Callback function that is invoked on progress (can be empty)
    *
    *  @return
    *    'true', if storage was successful, esle 'false'
    */
    template <typename T>
    bool store(const std::string & filename, T * resource, const reflectionzeug::Variant & options = reflectionzeug::Variant(), std::function<void(int, int)> progress = std::function<void(int, int)>()) const;


protected:
    /**
    *  @brief
    *    Get file extension
    *
    *  @param[in] filename
    *    Path to file (with filename and extension)
    *
    *  @return
    *    Filename extension (lower-case)
    */
    std::string getFileExtension(const std::string & filename) const;


protected:
    std::vector<AbstractLoader *> m_loaders;    /**< Available loaders */
    std::vector<AbstractStorer *> m_storers;    /**< Available storers */
};


} // namespace gloperate


#include <gloperate/resources/ResourceManager.hpp>
