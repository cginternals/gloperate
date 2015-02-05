/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <unordered_map>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractMetaInformationCapability.h>


namespace gloperate {

class AbstractViewportCapability;

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API MetaInformationCapability : public AbstractMetaInformationCapability
{

public:
    /**
    *  @brief
    *    Constructor
    */
    MetaInformationCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MetaInformationCapability();

    /**
    *  @brief
    *    Returns meta information for the given id
    */
    virtual const std::string & get(int id) const override;

    void setMetaInformation(const std::unordered_map<int, std::string> & data);

    std::unordered_map<int, std::string> & data();
    const std::unordered_map<int, std::string> & data() const;

protected:
    std::unordered_map<int, std::string> m_data;
};


} // namespace gloperate
