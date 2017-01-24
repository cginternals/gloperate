
#pragma once


#include <iosfwd>

#include <cppexpose/plugin/plugin_api.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate/base/Loader.h>
#include <gloperate/gloperate-version.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{
class ResourceManager;
}


namespace gloperate_text
{


class FontFace;


/**
* @brief
*   The FontLoader provides interfaces to load font face descriptions from files.
*
*   It can be registered at a ResourceManager as a generic loader for font faces.
*/
class GLOPERATE_TEXT_API FontLoader : public gloperate::Loader<FontFace>
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
    *   File dialog filter for all loadable file types.
    */
    virtual std::vector<std::string> loadingTypes() const override;

    /**
    *  @brief
    *    Get file dialog filter for all loadable file types as one string.
    *
    *  @return
    *   File dialog filter for all loadable file types.
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
    virtual FontFace * load(
        const std::string & filename
    ,   const cppexpose::Variant & options = cppexpose::Variant()
    ,   std::function<void(int, int)> progress  = nullptr) const override;


protected:
    /**
    *  @brief
    *    Handle common info block of font face description file
    *
    *  @param[in]    stream
    *    The stream to read info from
    *  @param[inout] fontFace
    *    The font face to construct
    *  @param[out]   fontSize
    *    The retrieved font size of the font face
    */
    void handleInfo(std::stringstream & stream, FontFace & fontFace, float & fontSize) const;

    /**
    *  @brief
    *    Handle common block of font face description file
    *
    *  @param[in]    stream
    *    The stream to read info from
    *  @param[inout] fontFace
    *    The font face to construct
    *  @param[in]   fontSize
    *    The font size to correctly determine other metrics
    */
    void handleCommon(std::stringstream & stream, FontFace & fontFace, float fontSize) const;

    /**
    *  @brief
    *    Handle font face page block of font face description file
    *
    *  @param[in]    stream
    *    The stream to read info from
    *  @param[inout] fontFace
    *    The font face to construct
    *  @param[out]   filename
    *    The file name of the description file to derivate glyph texture atlas file paths
    */
    void handlePage(std::stringstream & stream, FontFace & fontFace
        , const std::string & filename) const;

    /**
    *  @brief
    *    Handle font face character block of font face description file
    *
    *  @param[in]    stream
    *    The stream to read info from
    *  @param[inout] fontFace
    *    The font face to construct
    */
    void handleChar(std::stringstream & stream, FontFace & fontFace) const;

    /**
    *  @brief
    *    Handle font face kerning block of font face description file
    *
    *  @param[in]    stream
    *    The stream to read info from
    *  @param[inout] fontFace
    *    The font face to construct
    */
    void handleKerning(std::stringstream & stream, FontFace & fontFace) const;

    /**
    *  @brief
    *    Key-Value pairs of the description file
    */
    using StringPairs = std::map<std::string, std::string>;

    /**
    *  @brief
    *    Extract all key-value pairs of one line in the description file
    *
    *  @param[in] stream
    *    The stream to read from
    *  @param[in] mandatoryKeys
    *    The list of mandatory keys required for successful extraction
    *
    *  @return
    *    The list of all extracted key-value pairs, empty if not all mandatory keys exist
    */
    static StringPairs readKeyValuePairs(
        std::stringstream & stream
    ,   const std::initializer_list<const char *> & mandatoryKeys);
};


} // namespace gloperate_text
