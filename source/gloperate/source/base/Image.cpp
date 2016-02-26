
#include <gloperate/base/Image.h>


namespace gloperate
{


Image::Image()
: m_format(Image::Format::Invalid)
, m_data(nullptr)
, m_width(0)
, m_height(0)
{
}

Image::Image(const Image::Format format, const int width, const int height)
: m_format(format)
, m_data(nullptr)
, m_width(width)
, m_height(height)
{
    if (format == Image::Format::RGB24)
    {
        m_data = new char[width * height * 3];
    }
}

Image::Image(const Image::Format format, char * data, const int width, const int height)
: m_format(format)
, m_data(data)
, m_width(width)
, m_height(height)
{
}

Image::~Image()
{
}

void Image::setImage(const Image::Format format, char * data, const int width, const int height)
{
    m_format = format;
    m_data = data;
    m_width = width;
    m_height = height;
}

Image::Format Image::format() const
{
    return m_format;
}

char * Image::data()
{
    return m_data;
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}


} // namespace gloperate
