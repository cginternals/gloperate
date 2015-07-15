
#pragma once


#include <vector>
#include <string>

#include <gloperate/resources/Loader.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace globjects
{
    class Texture;
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
class GLOPERATE_QT_API QtTextureLoader : public gloperate::Loader<globjects::Texture> 
{
public:
    /**
    *  @brief
    *    Constructor
    */
    QtTextureLoader();

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
    virtual globjects::Texture * load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */
};


} // namespace gloperate_qt
