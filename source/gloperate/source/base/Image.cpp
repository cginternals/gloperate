
#include <gloperate/base/Image.h>

#include <cppassist/logging/logging.h>

#include <algorithm>


using namespace gl;
using namespace cppassist;


namespace gloperate
{


int Image::channels(GLenum format)
{
    switch (format)
    {
        case GL_RGB:
            return 3;
        case GL_RGBA:
            return 4;
        default:
            return 0;
    }
}

int Image::bytes(GLenum type)
{
    switch (type)
    {
        case GL_BYTE:           return 1;
        case GL_UNSIGNED_BYTE:  return 1;
        case GL_SHORT:          return 2;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_INT:            return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_FLOAT:          return 4;
        case GL_DOUBLE:         return 8;
        default:                return 0;
    }
}

Image::Image()
: m_width(0)
, m_height(0)
, m_format(GL_INVALID_ENUM)
, m_type(GL_INVALID_ENUM)
, m_channels(0)
, m_bytes(0)
, m_dataSize(0)
, m_data(nullptr)
{
}

Image::Image(int width, int height, GLenum format, GLenum type)
: Image()
{
    allocate(width, height, format, type);
}

Image::Image(int width, int height, GLenum format, GLenum type, const char * data)
: Image()
{
    copyImage(width, height, format, type, data);
}

Image::Image(int width, int height, GLenum format, GLenum type, char * data)
: Image()
{
    setData(width, height, format, type, data);
}

Image::Image(const Image & other)
: Image()
{
    copyImage(other.width(), other.height(), other.format(), other.type(), other.data());
}

Image::Image(Image && other)
: Image()
{
    swap(*this, other);
}

Image::~Image()
{
    delete [] m_data;
}

Image & Image::operator=(Image other)
{
    swap(*this, other);

    return *this;
}

bool Image::empty() const
{
    return m_data == nullptr;
}

int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

GLenum Image::format() const
{
    return m_format;
}

GLenum Image::type() const
{
    return m_type;
}

int Image::channels() const
{
    return m_channels;
}

int Image::bytes() const
{
    return m_bytes;
}

const char * Image::data() const
{
    return m_data;
}

char * Image::data()
{
    return m_data;
}

void Image::clear()
{
    // Release image data
    if (m_data) {
        delete [] m_data;
    }

    // Reset image
    m_width    = 0;
    m_height   = 0;
    m_format   = GL_INVALID_ENUM;
    m_type     = GL_INVALID_ENUM;
    m_channels = 0;
    m_bytes    = 0;
    m_dataSize = 0;
    m_data     = nullptr;
}

void Image::allocate(int width, int height, GLenum format, GLenum type)
{
    clear();

    initializeImage(width, height, format, type);

    if (m_dataSize == 0)
    {
        critical() << "Image buffer creation failed.";
        return;
    }

    m_data = new char[m_dataSize];
}

void Image::copyImage(int width, int height, GLenum format, GLenum type, const char * data)
{
    clear();

    allocate(width, height, format, type);

    if (m_data == nullptr)
    {
        critical() << "Image buffer creation failed.";
        return;
    }

    std::copy_n(data, m_dataSize, m_data);
}

void Image::setData(int width, int height, GLenum format, GLenum type, char * data)
{
    clear();

    initializeImage(width, height, format, type);

    m_data = m_dataSize ? data : nullptr;
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

void Image::initializeImage(int width, int height, GLenum format, GLenum type)
{
    m_width    = width;
    m_height   = height;
    m_channels = channels(format);
    m_bytes    = bytes(type);
    m_dataSize = m_width * m_height * m_channels * m_bytes;
    m_format   = format;
    m_type     = type;
}


} // namespace gloperate
