#pragma once


#include <globjects/Texture.h>

#include <gloperate/resources/Loader.h>


namespace gloperate
{


class GLOPERATE_API GlrawTextureLoader : public Loader<globjects::Texture> {


    public:
        GlrawTextureLoader();
        virtual ~GlrawTextureLoader();

        virtual bool canLoad(const std::string & ext) const override;
        virtual std::vector<std::string> loadingTypes() const override;
        virtual std::string allLoadingTypes() const override;

        virtual globjects::Texture * load(const std::string & filename) const override;


};


} // namespace gloperate
