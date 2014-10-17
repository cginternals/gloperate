/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/AbstractLoader.h>


namespace gloperate
{


/**
*  @brief
*    Loader class template
*/
template <typename T>
class Loader : public AbstractLoader {


public:
    /**
    *  @brief
    *    Constructor
    */
    Loader();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Loader();

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
    virtual T * load(const std::string & filename) const = 0;


};


} // namespace gloperate


#include <gloperate/resources/Loader.hpp>
