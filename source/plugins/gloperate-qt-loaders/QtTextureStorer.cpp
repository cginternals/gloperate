
#include "QtTextureStorer.h"

#include <QString>
#include <QImage>
#include <QImageWriter>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>


CPPEXPOSE_COMPONENT(QtTextureStorer, gloperate::AbstractStorer)


QtTextureStorer::QtTextureStorer(gloperate::Environment * environment)
: gloperate::Storer<globjects::Texture>(environment)
{
    // Get list of supported file formats
    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    for (int i = 0; i < formats.size(); ++i) {
        std::string format = std::string(formats[i].data());
        m_extensions.push_back(std::string(".") + format);
        m_types.push_back(format + " image (*." + format + ")");
    }

    // Add entry that contains all supported file formats
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }
    m_types.push_back(std::string("Qt image formats (") + allTypes + ")");
}

QtTextureStorer::~QtTextureStorer()
{
}

bool QtTextureStorer::canStore(const std::string & ext) const
{
     // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> QtTextureStorer::storingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string QtTextureStorer::allStoringTypes() const
{
    // Compose list of all supported file extensions
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }

    // Return supported types
    return allTypes;
}

bool QtTextureStorer::store(const std::string & filename, const globjects::Texture * texture, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    if (!texture)
    {
        return false;
    }

    // Load image

    texture->bind();
    int width = texture->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
    int height = texture->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);

    if (width <= 0 || height <= 0)
    {
        return false;
    }

    QImage image(width, height, QImage::Format_RGB888);

    texture->bind();
    gl::glGetTexImage(texture->target(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, image.bits());

    return image.mirrored().save(QString::fromStdString(filename));
}
