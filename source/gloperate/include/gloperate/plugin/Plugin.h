
#pragma once


#include <string>

#include <reflectionzeug/variant/Variant.h>

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
    *    Get plugin info
    *
    *  @return
    *    Custom plugin info in JSON format, e.g. path to data relative to the plugin directory, "" by default
    *
    *  @remarks
    *    To set this information, create a file named PluginInfo.json in
    *    the directory that contains your plugin libraries and define all
    *    necessary information. For the path where data is found relative to that directory, e.g.:
    *      { "relDataPath": "../data" }
    */
    const reflectionzeug::Variant & pluginInfo() const;

    /**
    *  @brief
    *    Set plugin info
    *
    *  @param[in] pluginInfo
    *    Variant containing JSON object with custom plugin info
    *
    *  @remarks
    *    Do not call this function directly, let PluginManager do the magic
    */
    void setPluginInfo(const reflectionzeug::Variant & pluginInfo);


protected:
    std::string             m_type;         ///< Plugin type (e.g., "Painter")
    std::string             m_name;         ///< Plugin name
    std::string             m_description;  ///< Plugin description
    std::string             m_vendor;       ///< Vendor name
    std::string             m_version;      ///< Plugin version
    reflectionzeug::Variant m_pluginInfo;   ///< Plugin info, like relative data path
};


} // namespace gloperate
