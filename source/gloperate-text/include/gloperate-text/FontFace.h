
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


/**
*  @brief
*    The FontFace represents a specific face of a font,
*    i.e., all configuration required to render a font at a specific point size, boldness, italique-state and other properties.
*
*    This class contains a set of glyphs, the according texture atlas(ses) and general configurations, provided as key-value string registry.
*/
class GLOPERATE_TEXT_API FontFace : public globjects::Referenced
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  Constructs an empty FontFace without any configuration.
    *  Use ethe appropriate setters to configure the font.
    *
    *  The FontImporter provides the import of a font from a font configuration provided by http://kvazars.com/littera/.
    */
    FontFace();

    /**
     * @brief
     *   Destructor
     *
     *  Releases all glyphs, kernings and configurations of this font face.
     *  The glyph texture remains if the ref_ptr is shared.
     */
    virtual ~FontFace();

    /**
     * @brief glyphTexture
     * @return the texture object containing the texture atlas(ses) of all glyphs.
     */
    globjects::Texture * glyphTexture() const;

    /**
     * @brief setGlyphTexture
     *   Updates the associated glyph texture atlas.
     *
     * @param texture the new texture atlas of the glyphs
     */
    void setGlyphTexture(globjects::Texture * texture);

    /**
     * @brief configuration
     * @param key the key of the configuration
     * @param defaultValue the default value if the configuration key doesn't exist
     * @return the configuration identified with key. If key doesn't exist, return defaultValue.
     */
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
