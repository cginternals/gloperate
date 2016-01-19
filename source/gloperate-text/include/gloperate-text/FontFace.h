
#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/Referenced.h>
#include <globjects/Texture.h>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/Glyph.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API FontFace : public globjects::Referenced
{
public:
    FontFace();
    virtual ~FontFace();

    globjects::Texture * glyphTexture() const;
    void setGlyphTexture(globjects::Texture * texture);

    const std::string & configuration(const std::string & key, const std::string & defaultValue = "") const;
    void setConfiguration(const std::string & key, const std::string & value);

    Glyph & glyph(std::uint32_t index);
    const Glyph & glyph(std::uint32_t index) const;
    void addGlyph(const Glyph & glyph);

    int kerning(std::uint32_t glyph1, std::uint32_t glyph2) const;
    void setKerning(std::uint32_t glyph1, std::uint32_t glyph2, int kerning);

protected:
    globjects::ref_ptr<globjects::Texture> m_glyphTexture;
    std::unordered_map<std::string, std::string> m_configuration;
    std::unordered_map<std::uint32_t, Glyph> m_glyphConfiguration;
    std::map<std::pair<std::uint32_t, std::uint32_t>, int> m_kernings;
};


} // namespace gloperate_text
