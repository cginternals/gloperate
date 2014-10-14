#pragma once


#include <vector>
#include <string>

#include <globjects/Texture.h>

#include <gloperate/resources/Loader.h>
#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API QtTextureLoader : public gloperate::Loader<globjects::Texture> {


    public:
        QtTextureLoader();
        virtual ~QtTextureLoader();

        virtual bool canLoad(const std::string & ext) const;
        virtual std::vector<std::string> loadingTypes() const;
        virtual std::string allLoadingTypes() const;
        virtual globjects::Texture * load(const std::string & filename) const override;


    protected:
        std::vector<std::string> m_extensions;
        std::vector<std::string> m_types;


};


} // namespace gloperate_qt
