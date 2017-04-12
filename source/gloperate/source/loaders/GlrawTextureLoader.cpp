
#include <gloperate/loaders/GlrawTextureLoader.h>

#include <algorithm>

#include <cppassist/fs/FilePath.h>
#include <cppassist/fs/readfile.h>
#include <cppassist/fs/RawFile.h>
#include <cppassist/fs/DescriptiveRawFile.h>

#include <cppexpose/variant/Variant.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/base/RawFileNameSuffix.h>


CPPEXPOSE_COMPONENT(GlrawTextureLoader, gloperate::AbstractLoader)


GlrawTextureLoader::GlrawTextureLoader(gloperate::Environment * environment)
    : gloperate::Loader<globjects::Texture>(environment)
{
    m_extensions.push_back(".raw");
    m_extensions.push_back(".glraw");

    m_types.push_back("raw images (*.raw)");
    m_types.push_back("glraw images (*.glraw)");
}

GlrawTextureLoader::~GlrawTextureLoader()
{
}

bool GlrawTextureLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return std::find(m_extensions.begin(), m_extensions.end(), "." + ext) != m_extensions.end();
}

std::vector<std::string> GlrawTextureLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string GlrawTextureLoader::allLoadingTypes() const
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

globjects::Texture * GlrawTextureLoader::load(const std::string & filename, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    globjects::Texture * texture = nullptr;

    cppassist::FilePath filePath(filename);
    if (filePath.extension() == "glraw")
        texture = loadGLRawImage(filename);
    else if (filePath.extension() == "raw")
        texture = loadRawImage(filename);

    return texture;
}

globjects::Texture * GlrawTextureLoader::loadGLRawImage(const std::string & filename) const
{
    cppassist::DescriptiveRawFile rawFile;
    if (!rawFile.load(filename))
        return nullptr;

    const int w = rawFile.intProperty("width");
    const int h = rawFile.intProperty("height");

    //TODO this should be a unique ptr like this:
    //auto texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    globjects::Texture * texture = new globjects::Texture(gl::GL_TEXTURE_2D);

    if (rawFile.hasIntProperty("format"))
    {
        auto format = static_cast<gl::GLenum>(rawFile.intProperty("format"));
        auto type = static_cast<gl::GLenum>(rawFile.intProperty("type"));

        texture->image2D(
            0,
            gl::GL_RGBA8,
            w,
            h,
            0,
            format,
            type,
            rawFile.data()
        );
    }
    else
    {
        auto compressedFormat = static_cast<gl::GLenum>(rawFile.intProperty("compressedFormat"));
        auto size = rawFile.intProperty("size");

        texture->compressedImage2D(
            0,
            compressedFormat,
            w,
            h,
            0,
            size,
            rawFile.data()
        );
    }

    return texture;
}

globjects::Texture * GlrawTextureLoader::loadRawImage(const std::string & filename) const
{
    gloperate::RawFileNameSuffix suffix(filename);
    if (!suffix.isValid())
        return nullptr;

    //read file
    cppassist::RawFile rawFile;
    if (!rawFile.load(filename))
        return nullptr;

    //TODO this should be a unique ptr like this:
    //auto texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    globjects::Texture * texture = new globjects::Texture(gl::GL_TEXTURE_2D);

    if (!suffix.compressed())
    {
        texture->image2D(
            0,
            gl::GL_RGBA8,
            suffix.width(),
            suffix.height(),
            0,
            suffix.format(),
            suffix.type(),
            rawFile.data()
        );
    }
    else
    {
        texture->compressedImage2D(
            0,
            gl::GL_RGBA8,
            suffix.width(),
            suffix.height(),
            0,
            rawFile.size(),
            rawFile.data()
        );
    }

    return texture;
}
