
#pragma once


#include <string>
#include <vector>
#include <functional>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;
class AbstractLoader;
class AbstractStorer;


/**
*  @brief
*    Class to help loading/accessing resources (textures, ...)
*/
class GLOPERATE_API ResourceManager : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment (must NOT be null!)
    */
    ResourceManager(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    ~ResourceManager();

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
    T * load(const std::string & filename, const cppexpose::Variant & options = cppexpose::Variant(), std::function<void(int, int)> progress = std::function<void(int, int)>()) const;

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
    bool store(const std::string & filename, T * resource, const cppexpose::Variant & options = cppexpose::Variant(), std::function<void(int, int)> progress = std::function<void(int, int)>()) const;


protected:
    /**
    *  @brief
    *    Update list of available loaders and storers
    */
    void updateComponents() const;

    /**
    *  @brief
    *    Clear loaders and storers
    */
    void clearComponents() const;


protected:
    Environment                           * m_environment; ///< Gloperate environment (must NOT be null!)
    mutable std::vector<AbstractLoader *>   m_loaders;     ///< Available loaders
    mutable std::vector<AbstractStorer *>   m_storers;     ///< Available storers
};


} // namespace gloperate


#include <gloperate/base/ResourceManager.inl>
