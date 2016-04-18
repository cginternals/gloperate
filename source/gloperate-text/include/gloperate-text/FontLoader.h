
#pragma once

#include <string>
#include <iosfwd>

#include <reflectionzeug/variant/Variant.h>

#include <gloperate/resources/Loader.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{

class ResourceManager;

} // namespace gloperate


namespace gloperate_text
{


class FontFace;



class GLOPERATE_TEXT_API FontLoader : public gloperate::Loader<FontFace>
{
public:
    FontLoader(gloperate::ResourceManager & resourceManager);

    virtual bool canLoad(const std::string & ext) const;

    virtual std::vector<std::string> loadingTypes() const;
    virtual std::string allLoadingTypes() const;

    virtual FontFace * load(
        const std::string & filename
    ,   const reflectionzeug::Variant & options = reflectionzeug::Variant()
    ,   std::function<void(int, int)> progress  = std::function<void(int, int)>()) const;

protected:

    void handleInfo    (std::stringstream & stream, FontFace & fontFace, float & fontSize) const;
    void handleCommon  (std::stringstream & stream, FontFace & fontFace, float fontSize) const;
    void handlePage    (std::stringstream & stream, FontFace & fontFace
        , const std::string & filename) const;
    void handleChar    (std::stringstream & stream, FontFace & fontFace) const;
    void handleKerning (std::stringstream & stream, FontFace & fontFace) const;

    using StringPairs = std::map<std::string, std::string>;
    static StringPairs readKeyValuePairs(
        std::stringstream & stream
    ,   const std::initializer_list<const char *> & mandatoryKeys);


protected:

    gloperate::ResourceManager & m_resourceManager;

};


} // namespace gloperate_text
