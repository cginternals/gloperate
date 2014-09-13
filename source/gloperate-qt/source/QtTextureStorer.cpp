/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/QtTextureStorer.h>
#include <gloperate-qt/qt-includes-begin.h>
#include <QString>
#include <QImage>
#include <gloperate-qt/qt-includes-end.h>
#include <glbinding/gl/gl.h>
#include <globjects/Texture.h>


namespace gloperate_qt
{


/**
*  @brief
*    Constructor
*/
QtTextureStorer::QtTextureStorer()
: TextureStorer()
{
}

/**
*  @brief
*    Destructor
*/
QtTextureStorer::~QtTextureStorer()
{
}

/**
*  @brief
*    Check if this storer can store into a specific file type
*/
bool QtTextureStorer::canStore(const std::string & /*ext*/) const
{
    // [TODO] Get a list of file formats supported by Qt
    return true;
}

/**
*  @brief
*    Get list of file types for storing
*/
std::vector<std::string> QtTextureStorer::storingTypes() const
{
    // [TODO] Get a list of file formats supported by Qt
    static std::vector<std::string> fileTypes {
        "Qt image formats (*.*)"
    };

    return fileTypes;
}

/**
*  @brief
*    Get all file types for storing
*/
std::string QtTextureStorer::allStoringTypes() const
{
    // [TODO] Get a list of file formats supported by Qt
    return "*.*";
}

/**
*  @brief
*    Store data to a file
*/
bool QtTextureStorer::store(const std::string & filename, const globjects::Referenced * object) const
{
    // [TODO] Support RGBA format (alpha channel)
    // [TODO] Support other image formats, e.g., grayscale
    // [TODO] Support 3D images
    // [TODO] Image is upside-down
    // [TODO] Support layer selection

    const globjects::Texture * texture = dynamic_cast<const globjects::Texture *>(object);

    if (!texture)
    {
        return false;
    }

    // Load image

    int width = texture->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
    int height = texture->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);

    if (width <= 0 || height <= 0)
    {
        return false;
    }

    QImage image(width, height, QImage::Format_RGBA8888);

    texture->bind();
    gl::glGetTexImage(texture->target(), 0, gl::GL_BGRA, gl::GL_UNSIGNED_BYTE, image.bits());

    return image.save(QString::fromStdString(filename));
}


} // namespace gloperate_qt
