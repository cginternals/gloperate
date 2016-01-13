
#include <gloperate-text/FontImporter.h>

#include <fstream>
#include <sstream>
#include <string>
#include <functional>

#include <loggingzeug/logging.h>

#include <stringzeug/regex.h>

#include <gloperate-text/FontFace.h>


namespace
{

void extractKeyValuePairs(std::stringstream & stream, std::function<void(const std::string &, const std::string)> callback)
{
    std::string key;
    std::string value;

    while (stream)
    {
        if (std::getline(stream, key, '='))
        {
            if (std::getline(stream, value, ' '))
            {
                callback(key, value);
            }
        }
    }
}

} // namespace


namespace gloperate_text
{


FontFace * FontImporter::loadFont(const std::string & filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);

    if (!in)
    {
        return nullptr;
    }

    FontFace * font = new FontFace();

    std::string line;
    std::string identifier;
    while (std::getline(in, line))
    {
        /*
        std::vector<std::string> identifiers = stringzeug::extract(line, "^([^ ]+)");
        std::vector<std::vector<std::string>> kvPairs = stringzeug::extractMultiple(line, "([a-z]+)=([a-zA-Z0-9]+)"); // \"\,\-

        assert(identifiers.size() == 1);

        const std::string & identifier = identifiers[0];

        for (const auto & match : kvPairs)
        {
            assert(match.size() == 2);

            const std::string & key = match[0];
            const std::string & value = match[1];
        }*/

        std::stringstream ss(line);

        if (std::getline(ss, identifier, ' '))
        {
            extractKeyValuePairs(ss, [](const std::string & key, const std::string & value) {
                loggingzeug::debug() << key << " = " << value;
            });
            if (identifier == "info")
            {
                handleInfo(ss, font);
            }
            else if (identifier == "common")
            {
                handleCommon(ss, font);
            }
            else if (identifier == "page")
            {
                handlePage(ss, font);
            }
            else if (identifier == "chars")
            {
                handleChars(ss, font);
            }
            else if (identifier == "char")
            {
                handleChar(ss, font);
            }
            else if (identifier == "kernings")
            {
                handleKernings(ss, font);
            }
            else if (identifier == "kerning")
            {
                handleKerning(ss, font);
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }
    }

    return font;
}

void FontImporter::handleInfo(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handleCommon(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handlePage(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handleChars(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handleChar(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handleKernings(std::stringstream & stream, FontFace * font)
{

}

void FontImporter::handleKerning(std::stringstream & stream, FontFace * font)
{

}


} // namespace gloperate_text
