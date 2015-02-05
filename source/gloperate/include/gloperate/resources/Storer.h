#pragma once


#include <functional>

#include <gloperate/resources/AbstractStorer.h>


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
    *  @param[in] progress
    *    Callback function that is invoked on progress (can be empty)
    *
    *  @return
    *   'true' if storage was successful, else 'false'
    */
    virtual bool store(const std::string & filename, const T * object, std::function<void(int, int)> progress) const = 0;
};

} // namespace gloperate


#include <gloperate/resources/Storer.hpp>
