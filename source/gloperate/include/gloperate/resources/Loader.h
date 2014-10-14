#pragma once


#include <gloperate/resources/AbstractLoader.h>


namespace gloperate
{


template <typename T>
class Loader : public AbstractLoader {


public:
    Loader();
    virtual ~Loader();

    virtual T * load(const std::string & filename) const = 0;


};


} // namespace gloperate


#include <gloperate/resources/Loader.hpp>
