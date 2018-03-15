
#include <gloperate/loaders/GlrawTextureStorer.h>

#include <algorithm>
#include <fstream>
#include <sstream>

#include <cppassist/fs/FilePath.h>
#include <cppassist/fs/DescriptiveRawFile.h>
#include <cppassist/logging/logging.h>

#include <cppexpose/variant/Variant.h>

#include <glbinding/Meta.h>
#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace
{

    static const std::map<gl::GLenum, std::string> suffixesByFormat =
    {
        { gl::GL_RED,   "rh"   },
        { gl::GL_GREEN, "g"    },
        { gl::GL_BLUE,  "b"    },
        { gl::GL_RG,    "rg"   },
        { gl::GL_RGB,   "rgb"  },
        { gl::GL_BGR,   "bgr"  },
        { gl::GL_RGBA,  "rgba" },
        { gl::GL_BGRA,  "bgra" }
    };

    static const std::map<gl::GLenum, std::string> suffixesByType =
    {
        { gl::GL_UNSIGNED_BYTE, "ub" },
        { gl::GL_BYTE,          "b"  },
        { gl::GL_UNSIGNED_INT,  "ui" },
        { gl::GL_INT,           "i"  },
        { gl::GL_FLOAT,         "f"  },
    };

    static const std::map<gl::GLenum, std::string> suffixesByCompressedFormat =
    {
        { gl::GL_COMPRESSED_RED_RGTC1,                   "rgtc1-r"         },
        { gl::GL_COMPRESSED_SIGNED_RED_RGTC1,            "rgtc1-sr"        },
        { gl::GL_COMPRESSED_RG_RGTC2,                    "rgtc2-rg"        },
        { gl::GL_COMPRESSED_SIGNED_RG_RGTC2,             "rgtc2-srg"       },
        { gl::GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,         "bptc-rgba-unorm" },
        { gl::GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,   "bptc-rgb-sf"     },
        { gl::GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, "bptc-rgb-uf"     },
        { gl::GL_COMPRESSED_RGB_S3TC_DXT1_EXT,           "dxt1-rgb"        },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,          "dxt1-rgba"       },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,          "dxt3-rgba"       },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,          "dxt5-rgba"       }
    };

} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(GlrawTextureStorer, gloperate::AbstractStorer)


GlrawTextureStorer::GlrawTextureStorer(Environment * environment)
: Storer<globjects::Texture>(environment)
{
    m_extensions.push_back(".raw");
    m_extensions.push_back(".glraw");

    m_types.push_back("raw images (*.raw)");
    m_types.push_back("glraw images (*.glraw)");
}

GlrawTextureStorer::~GlrawTextureStorer()
{
}

bool GlrawTextureStorer::canStore(const std::string & ext) const
{
    // Check if file type is supported
    return std::find(m_extensions.begin(), m_extensions.end(), "." + ext) != m_extensions.end();
}

std::vector<std::string> GlrawTextureStorer::storingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string GlrawTextureStorer::allStoringTypes() const
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

bool GlrawTextureStorer::store(const std::string & filename, const globjects::Texture * texture, const cppexpose::Variant &, std::function<void(int, int)>) const
{
    cppassist::FilePath filePath(filename);
    if (filePath.extension() == "glraw")
        return storeGLRawImage(filename, texture);
    else if (filePath.extension() == "raw")
        return storeRawImage(filename, texture);
    // else
    return false;
}

bool GlrawTextureStorer::storeGLRawImage(const std::string & filename, const globjects::Texture * texture) const
{
    // Collect properties
    const auto width  = static_cast<size_t>(texture->getLevelParameter(0, gl::GL_TEXTURE_WIDTH));
    const auto height = static_cast<size_t>(texture->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT));

    const auto compressed = static_cast<bool>(texture->getLevelParameter(0, gl::GL_TEXTURE_COMPRESSED));
    const auto format = static_cast<gl::GLenum>(texture->getLevelParameter(0, gl::GL_TEXTURE_INTERNAL_FORMAT));

    auto internalType = static_cast<gl::GLenum>(texture->getLevelParameter(0, gl::GL_TEXTURE_RED_TYPE));
    if (internalType == gl::GL_UNSIGNED_NORMALIZED) internalType = gl::GL_UNSIGNED_BYTE;
    if (internalType == gl::GL_SIGNED_NORMALIZED)   internalType = gl::GL_BYTE;

    const auto type = internalType;

    std::vector<std::pair<std::string, int32_t>> properties; // Only int properties here

    properties.emplace_back("width" , width);
    properties.emplace_back("height", height);

    if (compressed)
    {
        properties.emplace_back("compressedFormat", static_cast<int32_t>(format));
    }
    else
    {
        properties.emplace_back("format", static_cast<int32_t>(format));
        properties.emplace_back("type", static_cast<int32_t>(type));
    }

    // Assemble header
    std::stringstream header;
    const auto propType = cppassist::DescriptiveRawFile::PropertyType::Integer;

    for (const auto & prop : properties)
    {
        header.write(reinterpret_cast<const char*>(&propType), sizeof(propType));
        header << prop.first << '\0';
        header.write(reinterpret_cast<const char*>(&prop.second), sizeof(prop.second));
    }

    // Get raw data
    std::vector<unsigned char> data;
    if (compressed)
        data = texture->getCompressedImage();
    else
        data = texture->getImage(0, format, type);

    // Output to file
    std::ofstream outFile{filename, std::ios::binary};

    if (!outFile)
    {
        cppassist::error("GlrawTextureStorer") << "Failed to open \"" << filename << "\" for writing";
        return false;
    }

    const auto signature = cppassist::DescriptiveRawFile::s_signature;
    const uint64_t dataOffset = static_cast<uint64_t>(header.tellp()) + sizeof(signature) + sizeof(dataOffset);

    outFile.write(reinterpret_cast<const char*>(&signature ), sizeof(signature )); // Signature
    outFile.write(reinterpret_cast<const char*>(&dataOffset), sizeof(dataOffset)); // Data offset
    outFile << header.str();                                                       // Header
    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());        // Data

    return true;
}

bool GlrawTextureStorer::storeRawImage(const std::string & filename, const globjects::Texture * texture) const
{
    const cppassist::FilePath filePath(filename);

    const auto width  = static_cast<size_t>(texture->getLevelParameter(0, gl::GL_TEXTURE_WIDTH));
    const auto height = static_cast<size_t>(texture->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT));

    const auto compressed = static_cast<bool>(texture->getLevelParameter(0, gl::GL_TEXTURE_COMPRESSED));
    const auto format = static_cast<gl::GLenum>(texture->getLevelParameter(0, gl::GL_TEXTURE_INTERNAL_FORMAT));

    std::stringstream suffixCollector;
    suffixCollector << "." << width << "." << height << ".";

    std::vector<unsigned char> data;

    if (compressed)
    {
        if (!suffixesByCompressedFormat.count(format))
        {
            cppassist::error("GLrawTextureStorer") << "Unrecognized compressed texture format " << std::hex << std::showbase << static_cast<unsigned int>(format) << " (" << glbinding::Meta::getString(format) << ")";
            return false;
        }
        suffixCollector << suffixesByCompressedFormat.at(format);

        data = texture->getCompressedImage();
    }
    else
    {
        auto type = static_cast<gl::GLenum>(texture->getLevelParameter(0, gl::GL_TEXTURE_RED_TYPE)); // assuming all components are stored using the same type

        if (type == gl::GL_UNSIGNED_NORMALIZED) type = gl::GL_UNSIGNED_BYTE;
        if (type == gl::GL_SIGNED_NORMALIZED)   type = gl::GL_BYTE;

        if (!suffixesByFormat.count(format))
        {
            cppassist::error("GLrawTextureStorer") << "Unrecognized texture format " << std::hex << std::showbase << static_cast<unsigned int>(format) << " (" << glbinding::Meta::getString(format) << ")";
            return false;
        }
        if (!suffixesByType.count(type))
        {
            cppassist::error("GLrawTextureStorer") << "Unrecognized texture type " << std::hex << std::showbase << static_cast<unsigned int>(type) << " (" << glbinding::Meta::getString(type) << ")";
            return false;
        }

        suffixCollector << suffixesByFormat.at(format) << "." << suffixesByType.at(type);

        data = texture->getImage(0, format, type);
    }

    const auto suffix = suffixCollector.str();
    const auto fullFilename = filePath.directoryPath() + filePath.baseName() + suffix + '.' + filePath.extension(); // .extension() does not include the dot

    std::ofstream outFile{fullFilename, std::ios::binary};

    if (!outFile)
    {
        cppassist::error("GlrawTextureStorer") << "Failed to open \"" << fullFilename << "\" for writing";
        return false;
    }

    outFile.write(reinterpret_cast<const char *>(data.data()), data.size());

    return true;
}


} // namespace gloperate
