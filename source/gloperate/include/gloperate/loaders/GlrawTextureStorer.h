
#pragma once


#include <vector>
#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Storer.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


/**
*  @brief
*    File loader for '.raw' files
*/
class GLOPERATE_API GlrawTextureStorer : public gloperate::Storer<globjects::Texture>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        GlrawTextureStorer, gloperate::AbstractStorer
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Store .raw files"
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
    GlrawTextureStorer(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GlrawTextureStorer();

    // Virtual gloperate::AbstractStorer functions
    virtual bool canStore(const std::string & ext) const override;
    virtual std::vector<std::string> storingTypes() const override;
    virtual std::string allStoringTypes() const override;

    // Virtual gloperate::Storer<globjects::Texture> functions
    virtual bool store(const std::string & filename, const globjects::Texture * texture, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    /**
    *  @brief
    *    Create Texture from .glraw file
    *
    *  @param[in] filename
    *    path of the .glraw file
    *
    *  @return
    *    Loaded texture, null on error
    */
    bool storeGLRawImage(const std::string & filename, const globjects::Texture * texture) const;

    /**
    *  @brief
    *    Create Texture from .raw file
    *
    *  @param[in] filename
    *    path of the .raw file
    *
    *  @return
    *    Loaded texture, null on error
    */
    bool storeRawImage(const std::string & filename, const globjects::Texture * texture) const;


protected:
    std::vector<std::string> m_extensions; ///< List of supported file extensions (e.g., ".bmp")
    std::vector<std::string> m_types;      ///< List of supported file types (e.g., "bmp image (*.bmp)")
};


} // namespace gloperate
