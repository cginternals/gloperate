
#pragma once

#include <string>
#include <iosfwd>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{

class ResourceManager;

} // namespace gloperate


namespace gloperate_text
{


class FontFace;

class GLOPERATE_TEXT_API FontImporter
{
public:
    FontImporter(gloperate::ResourceManager & resourceManager);

    FontFace * loadFont(const std::string & filename);

protected:
    void handleInfo(std::stringstream & stream, FontFace * font);
    void handleCommon(std::stringstream & stream, FontFace * font);
    void handlePage(std::stringstream & stream, FontFace * font, const std::string & filename);
    void handleChars(std::stringstream & stream, FontFace * font);
    void handleChar(std::stringstream & stream, FontFace * font);
    void handleKernings(std::stringstream & stream, FontFace * font);
    void handleKerning(std::stringstream & stream, FontFace * font);

protected:
    gloperate::ResourceManager & m_resourceManager;
};


} // namespace gloperate_text
