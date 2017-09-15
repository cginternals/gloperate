
#include <gloperate-qt/loaders/QtTextureLoader.h>

#include <QString>
#include <QImage>
#include <QImageReader>

#include <cppexpose/reflection/Variant.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>

#include <gloperate-qt/base/Converter.h>


using namespace gloperate_qt;


namespace gloperate_qt
{


CPPEXPOSE_COMPONENT(QtTextureLoader, gloperate::AbstractLoader)


QtTextureLoader::QtTextureLoader(gloperate::Environment * environment)
: gloperate::Loader<globjects::Texture>(environment)
{
    // Get list of supported file formats
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
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

QtTextureLoader::~QtTextureLoader()
{
}

bool QtTextureLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> QtTextureLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string QtTextureLoader::allLoadingTypes() const
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

globjects::Texture * QtTextureLoader::load(const std::string & filename, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    // Load image
    QImage image;
    if (image.load(QString::fromStdString(filename))) {
        // Convert image into RGBA format
        QImage converted = Converter::convert(image);

        // Create texture
        //TODO this "release" is ugly but to change it to use unique_ptr all Loaders and the resource manager must be changed
        globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D).release();
        texture->image2D(
            0,
            gl::GL_RGBA8,
            converted.width(),
            converted.height(),
            0,
            gl::GL_RGBA,
            gl::GL_UNSIGNED_BYTE,
            converted.constBits()
        );
        return texture;
    }

    // Could not load image
    return nullptr;
}


} // namespace gloperate_qt
