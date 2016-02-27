
#include <gloperate/base/Image.h>

#include <globjects/base/baselogging.h>

#include <algorithm>


using namespace globjects;


namespace gloperate
{


Image::Image()
: m_data(nullptr)
, m_dataSize(0)
, m_width(0)
, m_height(0)
, m_format(Image::Format::Invalid)
{
}

Image::Image(const Image & other)
: Image()
{
    setData(other.cdata(), other.width(), other.height(), other.format());
}

Image::Image(Image && other)
: Image()
{
    swap(*this, other);
}

Image::Image(const int width, const int height, const Image::Format format)
: Image()
{
    createBuffer(width, height, format);
}

Image::Image(const char * data, const int width, const int height, const Image::Format format)
: Image()
{
    setData(data, width, height, format);
}

Image::Image(char * data, const int width, const int height, const Image::Format format)
: Image()
{
    setData(data, width, height, format);
}

Image::~Image()
{
    delete[] m_data;
}

Image& Image::operator=(Image other)
{
    swap(*this, other);

    return *this;
}

void Image::createBuffer(const int width, const int height, const Image::Format format)
{
    if (m_data) delete[] m_data;

    setImageData(width, height, format);

    if (m_dataSize == 0)
    {
        critical() << "Buffer creation failed.";
        return;
    }

    m_data = new char[m_dataSize];
}

void Image::setData(const char * data, const int width, const int height, const Image::Format format)
{
    if (m_data) delete[] m_data;

    createBuffer(width, height, format);

    if (m_data == nullptr)
    {
        critical() << "Data buffer invalid.";
        return;
    }

    std::copy_n(data, m_dataSize, m_data);
}

void Image::setData(char * data, const int width, const int height, const Image::Format format)
{
    if (m_data) delete[] m_data;

    setImageData(width, height, format);

    m_data = m_dataSize ? data : nullptr;
}

void Image::setImageData(const int width, const int height, const Image::Format format)
{
    m_dataSize = computeDataSize(width, height, format);
    m_width = width;
    m_height = height;
    m_format = format;
}

int Image::computeDataSize(const int width, const int height, const Image::Format format)
{
    if (width <= 0 || height <= 0)
    {
        warning() << "Image dimensions invalid. computeDataSize() returns 0.";
        return 0;
    }

    if (format == Image::Format::RGB24)
    {
        return width * height * 3;
    } else {
        warning() << "Image format invalid. computeDataSize() returns 0.";
        return 0;
    }
}

bool Image::isNull() const
{
    return m_data == nullptr;
}

Image::Format Image::format() const
{
    return m_format;
}

char * Image::data() const
{
    return m_data;
}

const char * Image::cdata() const
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

void swap(Image & first, Image & second) noexcept
{
    using std::swap;

    swap(first.m_data, second.m_data);
    swap(first.m_dataSize, second.m_dataSize);
    swap(first.m_width, second.m_width);
    swap(first.m_height, second.m_height);
    swap(first.m_format, second.m_format);
}


} // namespace gloperate
