
#pragma once

#include <string>
#include <iosfwd>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class FontFace;

class GLOPERATE_TEXT_API FontImporter
{
public:
    static FontFace * loadFont(const std::string & filename);

protected:
    FontImporter();

    static void handleInfo(std::stringstream & stream, FontFace * font);
    static void handleCommon(std::stringstream & stream, FontFace * font);
    static void handlePage(std::stringstream & stream, FontFace * font);
    static void handleChars(std::stringstream & stream, FontFace * font);
    static void handleChar(std::stringstream & stream, FontFace * font);
    static void handleKernings(std::stringstream & stream, FontFace * font);
    static void handleKerning(std::stringstream & stream, FontFace * font);
};


} // namespace gloperate_text
