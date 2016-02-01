
#include <gloperate-text/FontFace.h>


namespace
{
    const gloperate_text::Glyph emptyGlyph;
} // namespace


namespace gloperate_text
{


FontFace::FontFace()
{
}

FontFace::~FontFace()
{
}

globjects::Texture * FontFace::glyphTexture() const
{
    return m_glyphTexture;
}

void FontFace::setGlyphTexture(globjects::Texture * texture)
{
    m_glyphTexture = texture;
}

const std::string & FontFace::configuration(const std::string & key, const std::string & defaultValue) const
{
    if (!m_configuration.count(key))
    {
        return defaultValue;
    }

    return m_configuration.at(key);
}

void FontFace::setConfiguration(const std::string & key, const std::string & value)
{
    m_configuration.emplace(key, value);
}

Glyph & FontFace::glyph(std::uint32_t index)
{
    if (m_glyphConfiguration.count(index) == 0)
    {
        Glyph glyph;
        glyph.setIndex(index);

        addGlyph(glyph);
    }

    return m_glyphConfiguration.at(index);
}

const Glyph & FontFace::glyph(std::uint32_t index) const
{
    if (m_glyphConfiguration.count(index) == 0)
    {
        return emptyGlyph;
    }

    return m_glyphConfiguration.at(index);
}

void FontFace::addGlyph(const Glyph & glyph)
{
    m_glyphConfiguration.emplace(glyph.index(), glyph);
}

int FontFace::kerning(std::uint32_t glyph1, std::uint32_t glyph2) const
{
    std::pair<std::uint32_t, std::uint32_t> key(std::min(glyph1, glyph2), std::max(glyph1, glyph2));

    if (m_kernings.count(key) == 0)
    {
        return 0;
    }

    return m_kernings.at(key);
}

void FontFace::setKerning(std::uint32_t glyph1, std::uint32_t glyph2, int kerning)
{
    if (kerning == 0)
    {
        return;
    }

    std::pair<std::uint32_t, std::uint32_t> key(std::min(glyph1, glyph2), std::max(glyph1, glyph2));

    m_kernings.emplace(key, kerning);
}


} // namespace gloperate_text
