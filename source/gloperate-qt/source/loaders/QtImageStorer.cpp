
#include <gloperate-qt/loaders/QtImageStorer.h>

#include <QString>
#include <QImage>
#include <QImageWriter>

#include <glbinding/gl/gl.h>

#include <gloperate/rendering/Image.h>


namespace gloperate_qt
{


CPPEXPOSE_COMPONENT(QtImageStorer, gloperate::AbstractStorer)


QtImageStorer::QtImageStorer(gloperate::Environment * environment)
: gloperate::Storer<gloperate::Image>(environment)
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

QtImageStorer::~QtImageStorer()
{
}

bool QtImageStorer::canStore(const std::string & ext) const
{
     // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> QtImageStorer::storingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string QtImageStorer::allStoringTypes() const
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

bool QtImageStorer::store(const std::string & filename, const gloperate::Image * image, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    if (!image)
    {
        return false;
    }

    // Load image

    int width = image->width();
    int height = image->height();

    if (width <= 0 || height <= 0)
    {
        return false;
    }

    QImage qtImage(reinterpret_cast<const uchar *>(image->data()), width, height, QImage::Format_RGB888);

    return qtImage.mirrored().save(QString::fromStdString(filename));
}


} // namespace gloperate_qt
