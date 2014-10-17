/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/AbstractStorer.h>


namespace gloperate
{


/**
*  @brief
*    Storer class template
*/
template <typename T>
class Storer : public AbstractStorer {


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
    *  @param filename
    *    File name
    *  @param object
    *    Resource
    *
    *  @return
    *   'true' if storage was successful, else 'false'
    */
    virtual bool store(const std::string & filename, const T * object) const = 0;


};


} // namespace gloperate


#include <gloperate/resources/Storer.hpp>
