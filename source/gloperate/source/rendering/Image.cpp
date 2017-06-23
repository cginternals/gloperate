
#include <gloperate/rendering/Image.h>

#include <algorithm>

#include <cppassist/logging/logging.h>
#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>


using namespace gl;


namespace gloperate
{


int Image::channels(GLenum format)
{
    switch (format)
    {
        case GL_RED:                                    return 1;
        case GL_GREEN:                                  return 1;
        case GL_BLUE:                                   return 1;
        case GL_RED_INTEGER:                            return 1;
        case GL_GREEN_INTEGER:                          return 1;
        case GL_BLUE_INTEGER:                           return 1;
        case GL_DEPTH_COMPONENT:                        return 1;
        case GL_STENCIL_INDEX:                          return 1;
        case GL_COLOR_INDEX:                            return 1;
        case GL_LUMINANCE:                              return 1;
      
        case GL_RG:                                     return 2;
        case GL_RG_INTEGER:                             return 2;
        case GL_DEPTH_STENCIL:                          return 2;
        case GL_LUMINANCE_ALPHA:                        return 2;
                       
        case GL_RGB:                                    return 3;
        case GL_BGR:                                    return 3;
        case GL_RGB_INTEGER:                            return 3;
        case GL_BGR_INTEGER:                            return 3;
                       
        case GL_RGBA:                                   return 4;
        case GL_BGRA:                                   return 4;
        case GL_RGBA_INTEGER:                           return 4;
        case GL_BGRA_INTEGER:                           return 4;
                       
        default:                                        return 0;
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

Image::Image(std::string name)
: m_width(0)
, m_height(0)
, m_format(GL_INVALID_ENUM)
, m_type(GL_INVALID_ENUM)
, m_channels(0)
, m_bytes(0)
, m_dataSize(0)
, m_data(nullptr)
, m_name(name)
{
}

Image::Image(int width, int height, GLenum format, GLenum type, std::string name)
: Image(name)
{
    allocate(width, height, format, type);
}

Image::Image(int width, int height, GLenum format, GLenum type, const char * data, std::string name)
: Image(name)
{
    copyImage(width, height, format, type, data);
}

Image::Image(int width, int height, GLenum format, GLenum type, std::unique_ptr<char[]> && data, std::string name)
: Image(name)
{
    setData(width, height, format, type, std::move(data));
}

Image::Image(const Image & other)
: Image(other.name())
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

std::string Image::name() const
{
    return m_name;
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
    return m_data.get();
}

char * Image::data()
{
    return m_data.get();
}

void Image::clear()
{
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
        cppassist::critical() << "Image buffer creation failed.";
        return;
    }

    m_data = cppassist::make_unique<char[]>(m_dataSize);
}

void Image::copyImage(int width, int height, GLenum format, GLenum type, const char * data)
{
    clear();

    allocate(width, height, format, type);

    if (m_data == nullptr)
    {
        cppassist::critical() << "Image buffer creation failed.";
        return;
    }

    std::copy_n(data, m_dataSize, m_data.get());
}

void Image::setData(int width, int height, GLenum format, GLenum type, std::unique_ptr<char[]> && data)
{
    clear();

    initializeImage(width, height, format, type);

    m_data = m_dataSize ? std::move(data) : nullptr;
}

void swap(Image & first, Image & second) GLOPERATE_NOEXCEPT
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
