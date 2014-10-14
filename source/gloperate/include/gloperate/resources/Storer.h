#pragma once


#include <gloperate/resources/AbstractStorer.h>


namespace gloperate
{


template <typename T>
class Storer : public AbstractStorer {


public:
    Storer();
    virtual ~Storer();

    virtual bool store(const std::string & filename, const T * object) const = 0;


};


} // namespace gloperate


#include <gloperate/resources/Storer.hpp>
