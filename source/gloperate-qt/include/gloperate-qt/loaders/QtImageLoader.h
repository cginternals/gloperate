
#pragma once


#include <vector>
#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class Image;
}


namespace gloperate_qt
{


/**
*  @brief
*    Texture loader based on Qt
*
*  Supported options:
*    none
*/
class GLOPERATE_QT_API QtImageLoader : public gloperate::Loader<gloperate::Image>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        QtImageLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load images using the Qt image functionality"
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
    QtImageLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtImageLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual gloperate::Image * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; ///< List of supported file extensions (e.g., ".bmp")
    std::vector<std::string> m_types;      ///< List of supported file types (e.g., "bmp image (*.bmp)")
};


} // namespace gloperate_qt
