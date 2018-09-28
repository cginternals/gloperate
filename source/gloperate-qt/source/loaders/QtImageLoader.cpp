
#include <gloperate-qt/loaders/QtImageLoader.h>

#include <QString>
#include <QImage>
#include <QImageReader>

#include <cppexpose/variant/Variant.h>

#include <glbinding/gl/gl.h>

#include <gloperate/rendering/Image.h>

#include <gloperate-qt/base/Converter.h>


using namespace gloperate_qt;


namespace gloperate_qt
{


CPPEXPOSE_COMPONENT(QtImageLoader, gloperate::AbstractLoader)


QtImageLoader::QtImageLoader(gloperate::Environment * environment)
: gloperate::Loader<gloperate::Image>(environment)
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

QtImageLoader::~QtImageLoader()
{
}

bool QtImageLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> QtImageLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string QtImageLoader::allLoadingTypes() const
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

gloperate::Image * QtImageLoader::load(const std::string & filename, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    // Load image
    QImage image;
    if (image.load(QString::fromStdString(filename))) {
        // Convert image into RGBA format
        QImage converted = Converter::convert(image);

        // Create image
        return new gloperate::Image(converted.width(), converted.height(), gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, reinterpret_cast<const char*>(converted.constBits()));
    }

    // Could not load image
    return nullptr;
}


} // namespace gloperate_qt
