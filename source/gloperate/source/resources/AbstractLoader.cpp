/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/resources/AbstractLoader.h>
 

namespace gloperate
{


/**
*  @brief
*    Constructor
*/
AbstractLoader::AbstractLoader(ResourceType type)
: m_resourceType(type)
{
}

/**
*  @brief
*    Destructor
*/
AbstractLoader::~AbstractLoader()
{
}

/**
*  @brief
*    Get resource type supported by this loader
*/
AbstractLoader::ResourceType AbstractLoader::resourceType() const
{
    return m_resourceType;
}


} // namespace gloperate
