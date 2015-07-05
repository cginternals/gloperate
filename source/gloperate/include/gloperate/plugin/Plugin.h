
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Painter;
class ResourceManager;


/**
*  @brief
*    Represents a plugin that can be loaded dynamically
*/
class GLOPERATE_API Plugin 
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Plugin type (e.g., "painter")
    *  @param[in] name
    *    Plugin name
    *  @param[in] description
    *    Plugin description
    *  @param[in] vendor
    *    Vendor name
    *  @param[in] version
    *    Plugin version
    */
    Plugin(
        const std::string & type
    ,   const std::string & name
    ,   const std::string & description
    ,   const std::string & vendor
    ,   const std::string & version);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Plugin();

    /**
    *  @brief
    *    Get type
    *
    *  @return
    *    Plugin type (e.g., "painter")
    */
    const char * type() const;

    /**
    *  @brief
    *    Get name
    *
    *  @return
    *    Plugin name
    */
    const char * name() const;

    /**
    *  @brief
    *    Get description
    *
    *  @return
    *    Plugin description
    */
    const char * description() const;

    /**
    *  @brief
    *    Get vendor
    *
    *  @return
    *    Vendor name
    */
    const char * vendor() const;

    /**
    *  @brief
    *    Get version
    *
    *  @return
    *    Plugin version
    */
    const char * version() const;

    /**
    *  @brief
    *    Create painter
    *
    *  @param[in] resourceManager
    *    Resource manager for the painter to load external data
    *
    *  @return
    *    Pointer to newly created Painter, nullptr on error
    */
    virtual Painter * createPainter(ResourceManager & resourceManager) const = 0;


protected:
    std::string m_type;         ///< Plugin type (e.g., "painter")
    std::string m_name;         ///< Plugin name
    std::string m_description;  ///< Plugin description
    std::string m_vendor;       ///< Vendor name
    std::string m_version;      ///< Plugin version
};


} // namespace gloperate
