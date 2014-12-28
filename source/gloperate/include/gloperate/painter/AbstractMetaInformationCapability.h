/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractCapability.h>


namespace gloperate {

class AbstractViewportCapability;

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API AbstractMetaInformationCapability : public AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractMetaInformationCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractMetaInformationCapability();

    /**
    *  @brief
    *    Returns meta information for the given id
    */
    virtual const std::string & get(int id) const = 0;
};


} // namespace gloperate
