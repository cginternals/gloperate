
#pragma once


#include <vector>
#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/base/Loader.h>


namespace cpplocate {
    class ModuleInfo;
}

namespace globjects {
    class Texture;
}


/**
*  @brief
*    Texture loader based on Qt
*
*  Supported options:
*    none
*/
class QtTextureLoader : public gloperate::Loader<globjects::Texture> 
{
CPPEXPOSE_DECLARE_COMPONENT(QtTextureLoader, gloperate::AbstractLoader)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] moduleInfo
    *    Module info of the plugin library containing the component (if any)
    */
    QtTextureLoader(const cpplocate::ModuleInfo & moduleInfo);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtTextureLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const;
    virtual std::vector<std::string> loadingTypes() const;
    virtual std::string allLoadingTypes() const;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual globjects::Texture * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */
};
