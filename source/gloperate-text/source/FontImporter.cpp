
#include <gloperate-text/FontImporter.h>

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <set>
#include <algorithm>

#include <loggingzeug/logging.h>

#include <stringzeug/conversion.h>

#include <iozeug/FilePath.h>

#include <gloperate/resources/ResourceManager.h>

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

std::string stripped(const std::string & string, const std::set<char> & blacklist)
{
    std::string result = string;

    result.erase(std::remove_if(result.begin(), result.end(), [&blacklist] (char x) { return blacklist.count(x) > 0; } ), result.end());

    return result;
}

} // namespace


namespace gloperate_text
{


FontImporter::FontImporter(gloperate::ResourceManager & resourceManager)
: m_resourceManager(resourceManager)
{
}

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
        std::stringstream ss(line);

        if (std::getline(ss, identifier, ' '))
        {
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
                handlePage(ss, font, filename);
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

    if (!font->glyphTexture())
    {
        delete font;

        return nullptr;
    }

    return font;
}

void FontImporter::handleInfo(std::stringstream & stream, FontFace * font)
{
    extractKeyValuePairs(stream, [font](const std::string & key, const std::string & value) {
        font->setConfiguration(key, value);
    });
}

void FontImporter::handleCommon(std::stringstream & stream, FontFace * font)
{
    extractKeyValuePairs(stream, [font](const std::string & key, const std::string & value) {
        font->setConfiguration(key, value);
    });
}

void FontImporter::handlePage(std::stringstream & stream, FontFace * font, const std::string & filename)
{
    const std::string path = iozeug::FilePath(filename).directoryPath();
    extractKeyValuePairs(stream, [this, font, &path](const std::string & key, const std::string & value) {
        if (key == "file")
        {
            std::string filename = stripped(value, { '"', '\r' });

            font->setGlyphTexture(m_resourceManager.load<globjects::Texture>(path + "/" + filename));
        }
        else if (key == "id")
        {
            // nothing for now
        }
    });
}

void FontImporter::handleChars(std::stringstream & stream, FontFace * font)
{
    // nothing
}

void FontImporter::handleChar(std::stringstream & stream, FontFace * font)
{
    Glyph glyph;

    extractKeyValuePairs(stream, [&glyph](const std::string & key, const std::string & value) {
        std::uint32_t number = stringzeug::fromString<std::uint32_t>(value);

        if (key == "id")
        {
            glyph.setIndex(number);
        }
        else if (key == "x")
        {
            glyph.setX(number);
        }
        else if (key == "y")
        {
            glyph.setY(number);
        }
        else if (key == "width")
        {
            glyph.setWidth(number);
        }
        else if (key == "height")
        {
            glyph.setHeight(number);
        }
        else if (key == "xoffset")
        {
            glyph.setXOffset(number);
        }
        else if (key == "yoffset")
        {
            glyph.setYOffset(number);
        }
        else if (key == "xadvance")
        {
            glyph.setXAdvance(number);
        }
        else if (key == "page")
        {
            glyph.setPage(number);
        }
        else if (key == "chnl")
        {
            glyph.setChannel(number);
        }
    });

    if (glyph.index() > 0)
    {
        font->addGlyph(glyph);
    }
}

void FontImporter::handleKernings(std::stringstream & stream, FontFace * font)
{
    // nothing
}

void FontImporter::handleKerning(std::stringstream & stream, FontFace * font)
{
    std::uint32_t first = 0;
    std::uint32_t second = 0;
    int amount = 0;

    extractKeyValuePairs(stream, [this, font, &first, &second, &amount](const std::string & key, const std::string & value) {
        if (key == "first")
        {
            first = stringzeug::fromString<std::uint32_t>(value);
        }
        else if (key == "second")
        {
            second = stringzeug::fromString<std::uint32_t>(value);
        }
        else if (key == "amount")
        {
            // nothing for now
            amount = stringzeug::fromString<int>(value);
        }
    });
}


} // namespace gloperate_text
