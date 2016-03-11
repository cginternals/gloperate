
#include <gloperate/base/Image.h>

#include <globjects/base/baselogging.h>

#include <algorithm>


using namespace gl;
using namespace globjects;


namespace gloperate
{


Image::Image()
: m_data(nullptr)
, m_dataSize(0)
, m_width(0)
, m_height(0)
, m_channels(0)
, m_bytes(0)
, m_format(GL_INVALID_ENUM)
, m_type(GL_INVALID_ENUM)
{
}

Image::Image(const Image & other)
: Image()
{
    setData(other.data(), other.width(), other.height(), other.format(), other.type());
}

Image::Image(Image && other)
: Image()
{
    swap(*this, other);
}

Image::Image(int width, int height, GLenum format, GLenum type)
: Image()
{
    createBuffer(width, height, format, type);
}

Image::Image(const char * data, int width, int height, GLenum format, GLenum type)
: Image()
{
    setData(data, width, height, format, type);
}

Image::Image(char * data, int width, int height, GLenum format, GLenum type)
: Image()
{
    setData(data, width, height, format, type);
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

void Image::createBuffer(int width, int height, GLenum format, GLenum type)
{
    if (m_data) delete[] m_data;

    setImageData(width, height, format, type);

    if (m_dataSize == 0)
    {
        critical() << "Buffer creation failed.";
        return;
    }

    m_data = new char[m_dataSize];
}

void Image::setData(const char * data, int width, int height, GLenum format, GLenum type)
{
    if (m_data) delete[] m_data;

    createBuffer(width, height, format, type);

    if (m_data == nullptr)
    {
        critical() << "Data buffer invalid.";
        return;
    }

    std::copy_n(data, m_dataSize, m_data);
}

void Image::setData(char * data, int width, int height, GLenum format, GLenum type)
{
    if (m_data) delete[] m_data;

    setImageData(width, height, format, type);

    m_data = m_dataSize ? data : nullptr;
}

void Image::setImageData(int width, int height, GLenum format, GLenum type)
{
    m_width = width;
    m_height = height;
    m_channels = channels(format);
    m_bytes = bytes(type);
    m_dataSize = m_width * m_height * m_channels * m_bytes;
    m_format = format;
    m_type = type;
}

int Image::channels(GLenum format)
{
    switch (format)
    {
        case GL_RGB:
            return 3;
        case GL_RGBA:
            return 4;

        default:
        {
            warning() << "Image format not supported. Using channel size 0.";
            return 0;
        }
    }
}

int Image::bytes(GLenum type)
{
    switch (type)
    {
        case GL_UNSIGNED_BYTE: return 1;

        default:
        {
            warning() << "Image type not supported. Using bytes per channel: 0.";
            return 0;
        }
    }
}

bool Image::isNull() const
{
    return m_data == nullptr;
}

GLenum Image::format() const
{
    return m_format;
}

GLenum Image::type() const
{
    return m_type;
}

char * Image::data()
{
    return m_data;
}

const char * Image::data() const
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

int Image::channels() const
{
    return m_channels;
}

int Image::bytes() const
{
    return m_bytes;
}

void swap(Image & first, Image & second) noexcept
{
    using std::swap;

    swap(first.m_data, second.m_data);
    swap(first.m_dataSize, second.m_dataSize);
    swap(first.m_width, second.m_width);
    swap(first.m_height, second.m_height);
    swap(first.m_channels, second.m_channels);
    swap(first.m_bytes, second.m_bytes);
    swap(first.m_format, second.m_format);
    swap(first.m_type, second.m_type);
}


} // namespace gloperate
