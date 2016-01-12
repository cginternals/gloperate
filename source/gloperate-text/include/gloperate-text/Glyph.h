
#pragma once

#include <stddef.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API Glyph
{
public:
    Glyph();
    virtual ~Glyph();

    size_t id() const;
    void setId(size_t id);

    size_t x() const;
    void setX(size_t x);

    size_t y() const;
    void setY(size_t y);

    size_t width() const;
    void setWidth(size_t width);

    size_t height() const;
    void setHeight(size_t height);

    size_t xOffset() const;
    void setXOffset(size_t xOffset);
protected:
    size_t m_id;
    size_t m_x;
    size_t m_y;
    size_t m_width;
    size_t m_height;
    size_t m_xOffset;
    size_t m_yOffset;
    size_t m_xAdvance;
    //size_t m_page;
    //size_t m_channel;
};


} // namespace gloperate_text
