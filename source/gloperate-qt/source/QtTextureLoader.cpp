/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/QtTextureLoader.h>
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
QtTextureLoader::QtTextureLoader()
: TextureLoader()
{
}

/**
*  @brief
*    Destructor
*/
QtTextureLoader::~QtTextureLoader()
{
}

/**
*  @brief
*    Check if this loader can load a specific file type
*/
bool QtTextureLoader::canLoad(const std::string & /*ext*/) const
{
    // [TODO] Get a list of file formats supported by Qt
    return true;
}

/**
*  @brief
*    Get list of file types for loading
*/
std::vector<std::string> QtTextureLoader::loadingTypes() const
{
    // [TODO] Get a list of file formats supported by Qt
    static std::vector<std::string> fileTypes {
        "Qt image formats (*.*)"
    };

    return fileTypes;
}

/**
*  @brief
*    Get all file types for loading
*/
std::string QtTextureLoader::allLoadingTypes() const
{
    // [TODO] Get a list of file formats supported by Qt
    return "*.*";
}

/**
*  @brief
*    Load data from file
*/
globjects::Referenced * QtTextureLoader::load(const std::string & filename) const
{
    // [TODO] Support RGBA format (alpha channel)
    // [TODO] Support other image formats, e.g., grayscale
    // [TODO] Support 3D images
    // [TODO] Image is upside-down

    // Load image
    QImage image;
    if (image.load(QString::fromStdString(filename))) {
        // Convert image into RGBA format
        QImage converted = image.convertToFormat(QImage::QImage::Format_RGB888);

        // Create texture
        globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
        texture->image2D(
            0,
            gl::GL_RGB,
            converted.width(),
            converted.height(),
            0,
            gl::GL_RGB,
            gl::GL_UNSIGNED_BYTE,
            converted.constBits()
        );
        return texture;
    }

    // Could not load image
    return nullptr;
}


} // namespace gloperate_qt
