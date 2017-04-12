
#pragma once


#include <vector>
#include <string>


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>


namespace globjects
{
    class Texture;
}


/**
*  @brief
*    .raw file loader 
*/
class GlrawTextureLoader : public gloperate::Loader<globjects::Texture>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        GlrawTextureLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load .raw files"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the loader belongs (must NOT be null!)
    */
    GlrawTextureLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GlrawTextureLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual globjects::Texture * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;

protected:
    /**
    *  @brief
    *    create Texture from .glraw file
    *
    *    This function is called by load when the file extension is .glraw
    *
    *  @param[in] filename
    *    path of the .glraw file
    */
    globjects::Texture * loadGLRawImage(const std::string & filename) const;

    /**
    *  @brief
    *    create Texture from .raw file
    *
    *    This function is called by load when the file extension is .raw
    *
    *  @param[in] filename
    *    path of the .raw file
    */
    globjects::Texture * loadRawImage(const std::string & filename) const;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */
};
