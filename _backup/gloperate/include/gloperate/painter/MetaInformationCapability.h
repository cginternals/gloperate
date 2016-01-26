
#pragma once


#include <unordered_map>

#include <gloperate/painter/AbstractMetaInformationCapability.h>


namespace gloperate
{


class AbstractViewportCapability;


/**
*  @brief
*    Default implementation for AbstractMetaInformationCapability
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

    // Virtual functions from AbstractMetaInformationCapability
    virtual const std::string & get(int id) const override;

    /**
    *  @brief
    *    Get all meta information
    *
    *  @return
    *    Meta information map (ID -> string)
    */
    const std::unordered_map<int, std::string> & data() const;

    /**
    *  @brief
    *    Get all meta information
    *
    *  @return
    *    Meta information map (ID -> string)
    */
    std::unordered_map<int, std::string> & data();

    /**
    *  @brief
    *    Set all meta information
    *
    *  @param[in] data
    *    Meta information map (ID -> string)
    */
    void setMetaInformation(const std::unordered_map<int, std::string> & data);


protected:
    std::unordered_map<int, std::string> m_data;    /**< Meta information map (ID -> string) */
};


} // namespace gloperate
