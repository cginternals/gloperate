
#pragma once

#include <string>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class Font;

class GLOPERATE_TEXT_API FontImporter
{
public:
    static Font * loadFont(const std::string & filename);

protected:
    FontImporter();
};


} // namespace gloperate_text
