
#pragma once


#include <cstdint>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


using GlyphIndex = std::uint32_t; ///< Index type of a glyph in a FontFace


/**
*  @brief
*   Glyph related data for glyph based text rendering.
*
*   Most of the glyph data (except the advance) refers to the font 
*   face's glyph-texture.
*
*   Note: This class does not provide dpi awareness. This has to be
*   handled outside of this class, e.g., during layouting and rendering.
*/
class GLOPERATE_TEXT_API Glyph
{
public:
    using KerningBySubsequentGlyphIndex = std::unordered_map<GlyphIndex, float>; ///< Map type for kerning information lookup


public:
    /**
    *  @brief
    *    Constructor
    */
    Glyph();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Glyph();

    /**
    *  @brief
    *    Get index in associated FontFace
    *
    *  @return
    *    Index in FontFace
    */
    GlyphIndex index() const;

    /**
    *  @brief
    *    Set index in associated FontFace
    *
    *  @param[in] index
    *    The index in the associated FontFace
    */
    void setIndex(GlyphIndex index);

    /**
    *  @brief
    *    Get upper left position of the glyph's sub-texture.
    *
    *    The upper left position refers to the glyph-texture that is
    *    specified by a font face (see FontFace). It is the u and v
    *    coordinates poiting to the glyphs sub-texture within the
    *    texture atlas. The coordinates are normalized in [0;1].
    *
    *  @return
    *    Normalized coordinates pointing to the upper left texel of
    *    the glyph's sub-texture.
    */
    const glm::vec2 & subTextureOrigin() const;

    /**
    *  @brief
    *    Set the upper left position of the glyph's sub-texture.
    *
    *  @param[in] origin
    *    The origin denotes the upper left position that refers to
    *    the glyph-texture that is specified by a font face (see
    *    FontFace). It is the u and v coordinates poiting to the
    *    glyphs sub-texture within the texture atlas. The
    *    coordinates are normalized in [0;1].
    */
    void setSubTextureOrigin(const glm::vec2 & origin);

    /**
    *  @brief
    *    Get width and height of the glyph's sub-texture.
    *
    *    In combination with the sub-texture offset (subTextureOffset)
    *    the sub-texture rectangle is implictily fully specified in
    *    normalized texture coordinates.
    *
    *  @return
    *    Normalized width and height of the glyph's sub-texture.
    *
    *  @remarks
    *    The extent comprises the font face's padding.
    */
    const glm::vec2 & subTextureExtent() const;

    /**
    *  @brief
    *    Set the width and height of the glyph's sub-texture.
    *
    *  @param[in] extent
    *    The extent, in combination with the sub-texture offset
    *    (subTextureOffset), implictily specifies the sub-texture
    *    rectangle in normalized texture coordinates.
    *
    *  @remarks
    *    the extent comprises the font face's padding.
    */
    void setSubTextureExtent(const glm::vec2 & extent);

    /**
    *  @brief
    *    Check if a glyph is depictable/renderable
    *
    *    If the glyph's subtexture vertical or horizontal extent is
    *    zero the glyph does not need to be depicted/rendered. E.g.,
    *    spaces, line feeds, other control sequences as well as
    *    unknown glyphs do not need to be processed for rendering.
    *
    *  @return
    *    Returns true if the glyph needs to be depicted/rendered.
    */
    bool depictable() const;

    /**
    *  @brief
    *    Get the x and y offsets w.r.t. to the pen-position on the baseline.
    *
    *    The horizontal bearing does not comprise the glyph-texture's
    *    padding provided by the owning font face (see FontFace).
    *    The vertical bearing also does not comprises the glyph-
    *    texture's padding and is the measured w.r.t. baseline.
    *
    *  @return
    *    The horizontal and vertical bearing based on the glyphs origin or
    *    the pen-position placed on the baseline in pt.
    */
    const glm::vec2 & bearing() const;

    /**
    *  @brief
    *    Set the x and y offsets w.r.t. to the pen-position on the baseline.
    *
    *    The horizontal bearing does not comprise the glyph-texture's
    *    padding provided by the owning font face (see FontFace).
    *    The vertical bearing also does not comprises the glyph-
    *    texture's padding and is the measured w.r.t. baseline.
    *
    *  @param[in] bearing
    *    The horizontal and vertical bearing based on the glyphs origin or
    *    the pen-position placed on the baseline in pt.
    */
    void setBearing(const glm::vec2 & bearing);

    /**
    *  @brief
    *    Convenience setter for the x and y bearings.
    *
    *    The horizontal bearing does not comprise the glyph-texture's
    *    padding provided by the owning font face (see FontFace).
    *    The vertical bearing also does not comprise the glyph-
    *    texture's padding and is the measured w.r.t. baseline.
    *
    *    The vertical bearing is computed as follows:
    *        bearingY = fontAscent - (yOffset - top padding)
    *    The horizontal bearing equals the xOffset:
    *        bearingX = xOffset - left padding:
    *
    *  @param[in] fontAscent
    *    The font face's (FontFace) ascent in pt.
    *  @param[in] xOffset
    *    The glyphs horizotnal offset without left padding.
    *  @param[in] yOffset
    *    The glyphs vertical offset w.r.t. the font's topmost
    *    descendends, without the font's top padding in pt.
    */
    void setBearing(float fontAscent, float xOffset, float yOffset);

    /**
    *  @brief
    *    Get width and height of the glyph in pt.
    *
    *  @return
    *    The glyph's extent by means of width and height in pt.
    */
    const glm::vec2 & extent() const;

    /**
    *  @brief
    *    Set width and height of the glyph in pt.
    *
    *  @param[in] extent
    *    The glyph's extent (width and height) in pt.
    */
    void setExtent(const glm::vec2 & extent);

    /**
    *  @brief
    *    Get the glyph's horizontal overall advance in pt.
    *
    *    The horizontal advance comprises the font face's left and
    *    right padding, the glyphs (inner) width as well as the
    *    horizontal bearing (and often a glyph specific gap). E.g.:
    *        advance = subtextureExtent_width + xOffset (+ gap)
    *    or alternatively:
    *        advance = xOffset + padding_left + glyph_width + padding_right (+ gap)
    *
    *  @return
    *    The glyphs horizontal advance (along the baseline) in pt.
    */
    float advance() const;

    /**
    *  @brief
    *    Set the glyph's horizontal overall advance in pt.
    *
    *    The horizontal advance comprises the font face's left and
    *    right padding, the glyphs (inner) width as well as the
    *    horizontal bearing (and often a glyph specific gap). E.g.:
    *        advance = subtextureExtent_width + xOffset (+ gap)
    *    or alternatively:
    *        advance = xOffset + padding_left + glyph_width + padding_right (+ gap)
    *
    *  @param[in] advance
    *    The glyphs horizontal advance (along the baseline) in pt.
    */
    void setAdvance(float advance);

    /**
    *  @brief
    *    Get the glyph's kernel w.r.t. a subsequent glyph in pt.
    *
    *    The kerning provides a(usually negative) offset along the
    *    baseline that can be used to move the pen-position respectively.
    *    i.e., the subsequent pen-position is computed as follows:
    *        pen-position + advance + kerning
    *
    *  @param[in] subsequentIndex
    *    The subsequent glyph's index.
    *
    *  @return
    *    The kerning w.r.t. to the subsequent glyph in pt. If no
    *    kerning data is available for the subsequent glyph, the return
    *    value is zero/no kerning.
    */
    float kerning(GlyphIndex subsequentIndex) const;

    /**
    *  @brief
    *    Set the glyph's kernel w.r.t. a subsequent glyph in pt.
    *
    *    The kerning provides a(usually negative) offset along the
    *    baseline that can be used to move the pen-position respectively.
    *    i.e., the subsequent pen-position is computed as follows:
    *        pen-position + advance + kerning
    *
    *  @param[in] subsequentIndex
    *    The subsequent glyph's index.
    *  @param[in] kerning
    *    The kerning value w.r.t. to the subsequent glyph in pt.
    *    Note: the kerning should be a negative value but is not
    *        enforced to be in terms of assertion or clamping.
    *    If kerning data for the subsequent glyph is already
    *    available it will be updated to the provided value.
    */
    void setKerning(GlyphIndex subsequentIndex, float kerning);


protected:
    GlyphIndex m_index; ///< Index in the associated FontFace

    glm::vec2 m_subtextureOrigin; ///< Upper left position of the glyph's sub-texture
    glm::vec2 m_subtextureExtent; ///< Width and height of the glyph's sub-texture

    glm::vec2 m_bearing; ///< x and y offsets w.r.t. to the pen-position on the baseline
    float     m_advance; ///< Glyph's horizontal overall advance in pt
    glm::vec2 m_extent;  ///< Width and height of the glyph in pt

    KerningBySubsequentGlyphIndex m_kernings; ///< Kerning information map with upcoming glyph as key
};


} // namespace gloperate_text
