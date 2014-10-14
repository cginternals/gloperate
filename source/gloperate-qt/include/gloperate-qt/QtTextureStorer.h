#pragma once


#include <globjects/Texture.h>

#include <gloperate/resources/Storer.h>
#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API QtTextureStorer : public gloperate::Storer<globjects::Texture> {


    public:
        QtTextureStorer();
        virtual ~QtTextureStorer();

        virtual bool canStore(const std::string & ext) const;
        virtual std::vector<std::string> storingTypes() const;
        virtual std::string allStoringTypes() const;
        virtual bool store(const std::string & filename, const globjects::Texture * texture) const override;


    protected:
        std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
        std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */


};


} // namespace gloperate_qt
