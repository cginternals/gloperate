
#pragma once


#include <string>

#include <cpplocate/ModuleInfo.h>

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
    , const std::string & name
    , const std::string & description
    , const std::string & vendor
    , const std::string & version);

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
    *    Get module info
    *
    *  @return
    *    Module information, read from modinfo file (if present)
    *
    *  @remarks
    *    To set this information, create a file named <libname>.modinfo in the same
    *    directory that contains your plugin libraries and define all necessary information.
    *    (libmyplugin.so -> libmyplugin.modinfo, or myplugin.dll -> myplugin.modinfo).
    *    For example, you should define the data path relative to that directory:
    *      { "dataPath": "${ModulePath}/../data" }
    */
    const cpplocate::ModuleInfo & moduleInfo() const;

    /**
    *  @brief
    *    Set module info
    *
    *  @param[in] modInfo
    *    Module information found next to the plugin library
    *
    *  @remarks
    *    Do not call this function directly, let PluginManager do the magic
    */
    void setModuleInfo(const cpplocate::ModuleInfo & moduleInfo);


protected:
    std::string           m_type;         ///< Plugin type (e.g., "Painter")
    std::string           m_name;         ///< Plugin name
    std::string           m_description;  ///< Plugin description
    std::string           m_vendor;       ///< Vendor name
    std::string           m_version;      ///< Plugin version
    cpplocate::ModuleInfo m_moduleInfo;   ///< Module information for the plugin library
};


} // namespace gloperate
