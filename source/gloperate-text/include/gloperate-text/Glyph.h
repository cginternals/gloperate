
#pragma once

#include <cstdint>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API Glyph
{
public:
    Glyph();
    virtual ~Glyph();

    std::uint32_t index() const;
    void setIndex(std::uint32_t index);

    std::uint32_t x() const;
    void setX(std::uint32_t x);

    std::uint32_t y() const;
    void setY(std::uint32_t y);

    std::uint32_t width() const;
    void setWidth(std::uint32_t width);

    std::uint32_t height() const;
    void setHeight(std::uint32_t height);

    std::uint32_t xOffset() const;
    void setXOffset(std::uint32_t xOffset);

    std::uint32_t yOffset() const;
    void setYOffset(std::uint32_t yOffset);

    std::uint32_t xAdvance() const;
    void setXAdvance(std::uint32_t xAdvance);

    std::uint32_t page() const;
    void setPage(std::uint32_t page);

    std::uint32_t channel() const;
    void setChannel(std::uint32_t channel);

protected:
    std::uint32_t m_index;
    std::uint32_t m_x;
    std::uint32_t m_y;
    std::uint32_t m_width;
    std::uint32_t m_height;
    std::uint32_t m_xOffset;
    std::uint32_t m_yOffset;
    std::uint32_t m_xAdvance;
    std::uint32_t m_page;
    std::uint32_t m_channel;
};


} // namespace gloperate_text
