
#pragma once


#include <vector>
#include <string>
#include <unordered_map>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>


namespace gloperate
{


class ColorGradientList;


/**
*  @brief
*    Color gradient loader
*
*  Supported options:
*    none
*/
class GLOPERATE_API ColorGradientLoader : public Loader<ColorGradientList>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorGradientLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load a color gradient from a source code file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Constructor
    */
    ColorGradientLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorGradientLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual ColorGradientList * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; ///< List of supported file extensions (e.g., ".bmp")
    std::vector<std::string> m_types;      ///< List of supported file types (e.g., "bmp image (*.bmp)")
};


} // namespace gloperate
