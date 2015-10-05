
#pragma once


#include <functional>

#include <gloperate/resources/AbstractStorer.h>


namespace reflectionzeug {
    class Variant;
}


namespace gloperate
{


/**
*  @brief
*    Storer class template
*/
template <typename T>
class Storer : public AbstractStorer 
{
public:
    /**
    *  @brief
    *    Constructor
    */
    Storer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Storer();

    /**
    *  @brief
    *    Store resource to file
    *
    *  @param[in] filename
    *    File name
    *  @param[in] object
    *    Resource
    *  @param[in] options
    *    Options for loading resource, see documentation of specific storer for supported options
    *  @param[in] progress
    *    Callback function that is invoked on progress (can be empty)
    *
    *  @return
    *   'true' if storage was successful, else 'false'
    */
    virtual bool store(const std::string & filename, const T * object, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const = 0;
};


} // namespace gloperate


#include <gloperate/resources/Storer.hpp>
