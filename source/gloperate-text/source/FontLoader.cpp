
#include <gloperate-text/FontLoader.h>

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <set>
#include <map>

#include <glbinding/gl/enum.h>

#include <cppassist/logging/logging.h>
#include <cppassist/string/conversion.h>
#include <cppassist/string/manipulation.h>
#include <cppassist/fs//FilePath.h>
#include <cppassist/fs/RawFile.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <gloperate-text/FontFace.h>


namespace gloperate_text
{


CPPEXPOSE_COMPONENT(FontLoader, gloperate::AbstractLoader)


FontLoader::FontLoader(gloperate::Environment * environment)
: gloperate::Loader<FontFace>(environment)
{
}

bool FontLoader::canLoad(const std::string & ext) const
{
    return ext == "fnt";
}

std::vector<std::string> FontLoader::loadingTypes() const
{
    return { "Bitmap Font (*.fnt)" };
}

std::string FontLoader::allLoadingTypes() const
{
    return "*.fnt";
}


FontFace * FontLoader::load(const std::string & filename
    , const cppexpose::Variant &, const std::function<void(int, int)>) const
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);

    if (!in)
        return nullptr;

    auto fontFace = new FontFace();


    auto line = std::string();
    auto identifier = std::string();
    auto fontSize = 0.f;

    while (std::getline(in, line))
    {
        std::stringstream ss;
        ss << line;

        if (!std::getline(ss, identifier, ' '))
        {
            assert(false);
            continue;
        }

        if      (identifier == "info")
        {
            handleInfo(ss, *fontFace, fontSize);
        }
        else if (identifier == "common")
        {
            handleCommon(ss, *fontFace, fontSize);
        }
        else if (identifier == "page")
        {
            handlePage(ss, *fontFace, filename);
        }
        else if (identifier == "char")
        {
            handleChar(ss, *fontFace);
        }
        else if (identifier == "kerning")
        {
            handleKerning(ss, *fontFace);
        }
    }

    if (fontFace->glyphTexture())
        return fontFace;

    delete fontFace;
    return nullptr; 
}

void FontLoader::handleInfo(std::stringstream & stream, FontFace & fontFace, float & fontSize) const
{
    auto pairs = readKeyValuePairs(stream, { "size", "padding" });
  
    fontSize = cppassist::string::fromString<float>(pairs.at("size"));

    auto values = cppassist::string::split(pairs.at("padding"), ',');
    assert(values.size() == 4);

    auto padding = glm::vec4();
    padding[0] = cppassist::string::fromString<float>(values[2]); // top 
    padding[1] = cppassist::string::fromString<float>(values[1]); // right
    padding[2] = cppassist::string::fromString<float>(values[3]); // bottom
    padding[3] = cppassist::string::fromString<float>(values[0]); // left

    fontFace.setGlyphTexturePadding(padding);
}

void FontLoader::handleCommon(std::stringstream & stream, FontFace & fontFace, const float fontSize) const
{
    auto pairs = readKeyValuePairs(stream, { "lineHeight", "base", "scaleW", "scaleH" });

    fontFace.setAscent(cppassist::string::fromString<float>(pairs.at("base")));
    fontFace.setDescent(fontFace.ascent() - fontSize);

    assert(fontFace.size() > 0.f);
    fontFace.setLineHeight(cppassist::string::fromString<float>(pairs.at("lineHeight")));

    fontFace.setGlyphTextureExtent({
        cppassist::string::fromString<float>(pairs.at("scaleW")),
        cppassist::string::fromString<float>(pairs.at("scaleH")) });
}

void FontLoader::handlePage(std::stringstream & stream, FontFace & fontFace, const std::string & filename) const
{
    auto pairs = readKeyValuePairs(stream, { "file" });

    const auto path = cppassist::FilePath(filename).directoryPath();
    const auto file = cppassist::string::stripped(pairs.at("file"), { '"', '\r' });

    if (cppassist::string::hasSuffix(file, ".raw"))
    {
        auto texture = new globjects::Texture(gl::GL_TEXTURE_2D);
        auto raw = cppassist::RawFile();
        raw.load(path + "/" + file);

        if (!raw.isValid())
        {
            assert(false);
            return;
        }

        texture->image2D(0, gl::GL_R8, fontFace.glyphTextureExtent(), 0
            , gl::GL_RED, gl::GL_UNSIGNED_BYTE, raw.data());

        fontFace.setGlyphTexture(texture);
    }
    else
        fontFace.setGlyphTexture(m_environment->resourceManager()->load<globjects::Texture>(path + "/" + file));

    fontFace.glyphTexture()->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
    fontFace.glyphTexture()->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
    fontFace.glyphTexture()->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
    fontFace.glyphTexture()->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
}

void FontLoader::handleChar(std::stringstream & stream, FontFace & fontFace) const
{
    auto pairs = readKeyValuePairs(stream, { "id", "x", "y", "width", "height", "xoffset", "yoffset", "xadvance" });

    auto index = cppassist::string::fromString<GlyphIndex>(pairs.at("id"));
    assert(index > 0);

    auto glyph = Glyph();
    
    glyph.setIndex(index);

    const auto extentScale = 1.f / glm::vec2(fontFace.glyphTextureExtent());
    const auto extent = glm::vec2(
        cppassist::string::fromString<float>(pairs.at("width")),
        cppassist::string::fromString<float>(pairs.at("height")));

    glyph.setSubTextureOrigin({
        cppassist::string::fromString<float>(pairs.at("x")) * extentScale.x,
        1.f - (cppassist::string::fromString<float>(pairs.at("y")) + extent.y) * extentScale.y});

    glyph.setExtent(extent);
    glyph.setSubTextureExtent(extent * extentScale);

    glyph.setBearing(fontFace.ascent(), 
        cppassist::string::fromString<float>(pairs.at("xoffset")),
        cppassist::string::fromString<float>(pairs.at("yoffset")));

    glyph.setAdvance(cppassist::string::fromString<float>(pairs.at("xadvance")));

    fontFace.addGlyph(glyph);
}

void FontLoader::handleKerning(std::stringstream & stream, FontFace & fontFace) const
{
    auto pairs = readKeyValuePairs(stream, { "first", "second", "amount" });

    auto first = cppassist::string::fromString<GlyphIndex>(pairs.at("first"));
    assert(first > 0);

    auto second = cppassist::string::fromString<GlyphIndex>(pairs.at("second"));
    assert(second > 0);

    auto kerning = cppassist::string::fromString<float>(pairs.at("amount"));

    fontFace.setKerning(first, second, kerning);
}

FontLoader::StringPairs FontLoader::readKeyValuePairs(
    std::stringstream & stream
    , const std::initializer_list<const char *> & mandatoryKeys)
{
    auto key = std::string();
    auto value = std::string();

    auto pairs = StringPairs();

    while (stream)
    {
        if (!std::getline(stream, key, '='))
            continue;

        if (!std::getline(stream, value, ' '))
            continue;

        pairs.insert(std::pair<std::string, std::string>(key, value));
    }

    // check if all required keys are provided
    auto valid = true;
    for (const auto mandatoryKey : mandatoryKeys)
    {
        valid |= pairs.find(mandatoryKey) != pairs.cend();
    }

    if (!valid)
        return StringPairs();

    return pairs;
}


} // namespace gloperate_text
