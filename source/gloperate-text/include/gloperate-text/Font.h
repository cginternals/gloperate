
#pragma once

#include <unordered_map>
#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/Glyph.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API Font
{
public:
    Font();
    virtual ~Font();

    const globjects::Texture * glyphTexture() const;
    void setGlyphTexture(globjects::Texture * texture);

    const std::string & configuration(const std::string & key, const std::string & defaultValue = "") const;
    void setConfiguration(const std::string & key, const std::string & value);

    Glyph & glyph(size_t index);
    const Glyph & glyph(size_t index) const;
    void addGlyph(size_t index, const Glyph & glyph);

protected:
    globjects::ref_ptr<globjects::Texture> m_glyphs;
    std::unordered_map<std::string, std::string> m_configuration;
    std::unordered_map<size_t, Glyph> m_glyphConfiguration;
};


} // namespace gloperate_text
