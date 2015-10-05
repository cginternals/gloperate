
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


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
    *    Plugin type (e.g., "Painter")
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
    *    Get relative data path
    *
    *  @return
    *    Path to data relative to the plugin directory, "" by default
    *
    *  @remarks
    *    To set this information, create a file named PluginInfo.json in
    *    the directory that contains your plugin libraries and define where
    *    data is found relative to that directory, e.g.:
    *      { "relDataPath": "../data" }
    */
    const char * relDataPath() const;

    /**
    *  @brief
    *    Set relative data path
    *
    *  @param[in] path
    *    Path to data relative to the plugin directory, "" by default
    *
    *  @remarks
    *    Do not call this function directly, let PluginManager do the magic
    */
    void setRelDataPath(const char * path);


protected:
    std::string m_type;         ///< Plugin type (e.g., "Painter")
    std::string m_name;         ///< Plugin name
    std::string m_description;  ///< Plugin description
    std::string m_vendor;       ///< Vendor name
    std::string m_version;      ///< Plugin version
    std::string m_relDataPath;  ///< Relative path to data directory
};


} // namespace gloperate
