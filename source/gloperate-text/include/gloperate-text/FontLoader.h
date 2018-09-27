
#pragma once


#include <iosfwd>

#include <cppexpose/plugin/plugin_api.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate/base/Loader.h>
#include <gloperate/gloperate-version.h>

#include <gloperate-text/gloperate-text_api.h>


namespace openll
{
    class FontFace;
}


namespace gloperate
{
    class ResourceManager;
}


namespace gloperate_text
{


/**
* @brief
*   The FontLoader provides interfaces to load font face descriptions from files.
*
*   It can be registered at a ResourceManager as a generic loader for font faces.
*/
class GLOPERATE_TEXT_API FontLoader : public gloperate::Loader<openll::FontFace>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        FontLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load fonts in the Bitmap Font (BMFont) format"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Key-Value pairs of the description file
    */
    using StringPairs = std::map<std::string, std::string>;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    The environment.
    */
    explicit FontLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Check if this loader can load files with given extension.
    *
    *  @param[in] ext
    *    The extension to check
    *
    *  @return
    *    'true' if this loader can handle files of given extension, else 'false'
    */
    virtual bool canLoad(const std::string & ext) const override;

    /**
    *  @brief
    *    Get file dialog filter for all loadable file types.
    *
    *  @return
    *    File dialog filter for all loadable file types.
    */
    virtual std::vector<std::string> loadingTypes() const override;

    /**
    *  @brief
    *    Get file dialog filter for all loadable file types as one string.
    *
    *  @return
    *    File dialog filter for all loadable file types.
    */
    virtual std::string allLoadingTypes() const override;

    /**
    *  @brief
    *    Load a font face description file and create a new FontFace.
    *
    *  @param[in] filename
    *    The path to the font face description file
    *  @param[in] options
    *    A associative map with options to loading
    *  @param[in] progress
    *    A callback to propagate progress.
    *    The int parameters of the callback denote current and
    *    maximum required steps for loading.
    *
    *  @return
    *    A configured and initialized FontFace on success, else 'nullptr'
    */
    virtual openll::FontFace * load(
        const std::string & filename
    ,   const cppexpose::Variant & options = cppexpose::Variant()
    ,   std::function<void(int, int)> progress  = nullptr) const override;
};


} // namespace gloperate_text
