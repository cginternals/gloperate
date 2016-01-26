
#pragma once


#include <string>

#include <gloperate/painter/AbstractCapability.h>


namespace gloperate
{


class AbstractViewportCapability;


/**
*  @brief
*    Capability that provides meta information accessable by numeric IDs
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
    *    Get meta information for the given id
    *
    *  @param[in] id
    *    Information ID
    *
    *  @return
    *    Information string
    */
    virtual const std::string & get(int id) const = 0;
};


} // namespace gloperate
