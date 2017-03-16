
#pragma once


#include <vector>
#include <string>
#include <unordered_map>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>


namespace globjects
{
    class Shader;
}


/**
*  @brief
*    Shader loader based on Qt
*
*  Supported options:
*    none
*/
class ShaderLoader : public gloperate::Loader<globjects::Shader>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShaderLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load a shader from a source code file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Constructor
    */
    ShaderLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShaderLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const;
    virtual std::vector<std::string> loadingTypes() const;
    virtual std::string allLoadingTypes() const;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual globjects::Shader * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */

    const std::unordered_map<std::string, gl::GLenum> m_extensionToType; ///< Mapping of file extension to GLenum type
};
