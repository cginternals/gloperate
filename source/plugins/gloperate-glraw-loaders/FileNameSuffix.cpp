#include <FileNameSuffix.h>

#include <cassert>
#include <regex>

#include <glbinding/gl/enum.h>


namespace
{
    static const std::map<std::string, gl::GLenum> formatsBySuffix =
	{
        { "rh",     gl::GL_RED      },
        { "g",      gl::GL_GREEN    },
        { "b",      gl::GL_BLUE     },
        { "rg",     gl::GL_RG       },
        { "rgb",    gl::GL_RGB      },
        { "bgr",    gl::GL_BGR      },
        { "rgba",   gl::GL_RGBA     },
        { "bgra",   gl::GL_BGRA     }
	};

    static const std::map<std::string, gl::GLenum> typesBySuffix =
	{
        { "ub",             gl::GL_UNSIGNED_BYTE    },
        { "b",              gl::GL_BYTE             },
        { "us",             gl::GL_UNSIGNED_SHORT   },
        { "s",              gl::GL_SHORT            },
        { "ui",             gl::GL_UNSIGNED_INT     },
        { "i",              gl::GL_INT              },
        { "f",              gl::GL_FLOAT            },
        { "rgtc1-r",        gl::GL_COMPRESSED_RED_RGTC1,        },
        { "rgtc1-sr",       gl::GL_COMPRESSED_SIGNED_RED_RGTC1  },
        { "rgtc2-rg",       gl::GL_COMPRESSED_RG_RGTC2          },
        { "rgtc2-srg",      gl::GL_COMPRESSED_SIGNED_RG_RGTC2   },
        { "bptc-rgba-unorm",gl::GL_COMPRESSED_RGBA_BPTC_UNORM_ARB           },
        { "bptc-rgb-sf",    gl::GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB     },
        { "bptc-rgb-uf",    gl::GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB   },
        { "dxt1-rgb",       gl::GL_COMPRESSED_RGB_S3TC_DXT1_EXT   },
        { "dxt1-rgba",      gl::GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  },
        { "dxt3-rgba",      gl::GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  },
        { "dxt5-rgba",      gl::GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  }
	};
}


namespace glraw
{


FileNameSuffix::FileNameSuffix(const std::string & fileName)
: m_width (-1)
, m_height(-1)
, m_format(gl::GL_NONE)
, m_type(gl::GL_NONE)
, m_compressed(false)
{
	// check if either compressed or uncompressed (or unknown) format
    std::regex regexp{R"(^.*\.(\d+)\.(\d+)\.(\w+)\.raw$)"};
    std::smatch base_match;

    if (std::regex_match(fileName, base_match, regexp))
		m_compressed = true;
	else
	{
        regexp = std::regex{R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.raw$)"};
        if (!std::regex_match(fileName, base_match, regexp))
			return;
	}

	// retrieve intel from suffix parts
    try
    {
        m_width = std::stoi(base_match[1]);
        m_height = std::stoi(base_match[2]);
    }
    catch(...)
    {
        return;
    }

    if (!m_compressed)
    {
        m_format = format(base_match[3]);
        assert(m_format != gl::GL_NONE);
    }

    m_type = type(base_match[m_compressed ? 3 : 4]);
    assert(m_type != gl::GL_NONE);
}


bool FileNameSuffix::isValid() const
{
    return m_width  != -1
        && m_height != -1
        && m_type   != gl::GL_NONE
        && (m_compressed || m_format != gl::GL_NONE);
}

int FileNameSuffix::width() const
{
    return m_width;
}

int FileNameSuffix::height() const
{
    return m_height;
}

gl::GLenum FileNameSuffix::type() const
{
    return m_type;
}

gl::GLenum FileNameSuffix::format() const
{
    return m_format;
}

bool FileNameSuffix::compressed() const
{
    return m_compressed;
}

gl::GLenum FileNameSuffix::format(const std::string & format)
{
    std::string formatToLower = format;
    std::transform(formatToLower.begin(), formatToLower.end(), formatToLower.begin(), ::tolower);
    auto it = formatsBySuffix.find(formatToLower);
    return (it!=formatsBySuffix.end()) ? it->second : gl::GL_NONE;
}

gl::GLenum FileNameSuffix::type(const std::string & type)
{
    std::string typeToLower = type;
    std::transform(typeToLower.begin(), typeToLower.end(), typeToLower.begin(), ::tolower);
    auto it = typesBySuffix.find(typeToLower);
    return (it!=typesBySuffix.end()) ? it->second : gl::GL_NONE;
}

} // namespace glraw
