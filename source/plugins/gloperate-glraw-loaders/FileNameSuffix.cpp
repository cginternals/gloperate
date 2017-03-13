#include <FileNameSuffix.h>

#include <cassert>

#include <QMap>
#include <QRegExp>
#include <QStringList>

#include <glbinding/gl/gl.h>


namespace
{
    static const QMap<gl::GLenum, QString> suffixesByFormat =
	{
        { gl::GL_RED,   "r"    },
        { gl::GL_GREEN, "g"    },
        { gl::GL_BLUE,  "b"    },
        { gl::GL_RG,    "rg"   },
        { gl::GL_RGB,   "rgb"  },
        { gl::GL_BGR,   "bgr"  },
        { gl::GL_RGBA,  "rgba" },
        { gl::GL_BGRA,  "bgra" }
	};

    static const QMap<gl::GLenum, QString> suffixesByType =
	{
        { gl::GL_UNSIGNED_BYTE,  "ub" },
        { gl::GL_BYTE,           "b"  },
        { gl::GL_UNSIGNED_SHORT, "us" },
        { gl::GL_SHORT,          "s"  },
        { gl::GL_UNSIGNED_INT,   "ui" },
        { gl::GL_INT,            "i"  },
        { gl::GL_FLOAT,          "f"  }

    #ifdef gl::GL_ARB_texture_compression_rgtc
		,
        { gl::GL_COMPRESSED_RED_RGTC1,        "rgtc1-r"   },
        { gl::GL_COMPRESSED_SIGNED_RED_RGTC1, "rgtc1-sr"  },
        { gl::GL_COMPRESSED_RG_RGTC2,         "rgtc2-rg"  },
        { gl::GL_COMPRESSED_SIGNED_RG_RGTC2,  "rgtc2-srg" }
	#endif
    #ifdef gl::GL_ARB_texture_compression_bptc
		,
        { gl::GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,         "bptc-rgba-unorm" },
        { gl::GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,   "bptc-rgb-sf"     },
        { gl::GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, "bptc-rgb-uf"     }
	#endif
	#ifdef GLRAW_DXT // special treatment here - see S3TCExtensions.h
		,
        { gl::GL_COMPRESSED_RGB_S3TC_DXT1_EXT,  "dxt1-rgb"  },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, "dxt1-rgba" },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, "dxt3-rgba" },
        { gl::GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, "dxt5-rgba" }
	#endif
	};
}


namespace glraw
{

FileNameSuffix::FileNameSuffix(const QString & fileName)
: m_width (-1)
, m_height(-1)
, m_format(gl::GL_INVALID_ENUM)
, m_type(gl::GL_INVALID_ENUM)
, m_compressed(false)
{
	// check if either compressed or uncompressed (or unknown) format

	QRegExp regexp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.raw$)");
	if (regexp.exactMatch(fileName))
		m_compressed = true;
	else
	{
		regexp = QRegExp(R"(^.*\.(\d+)\.(\d+)\.(\w+)\.(\w+)\.raw$)");
		if (!regexp.exactMatch(fileName))
			return;
	}

	// retrieve intel from suffix parts

	QStringList parts = regexp.capturedTexts();

	bool ok;

	m_width = parts[1].toInt(&ok);
	assert(ok);

	m_height = parts[2].toInt(&ok);
	assert(ok);

	if (!m_compressed)
	{
		m_format = format(parts[3]);
        assert(m_format != gl::GL_INVALID_ENUM);
	}

	m_type = type(parts[m_compressed ? 3 : 4]);
    assert(m_type != gl::GL_INVALID_ENUM);
}

FileNameSuffix::FileNameSuffix(
    const int width, const int height, const gl::GLenum format, const gl::GLenum type)
: m_width(width)
, m_height(height)
, m_format(format)
, m_type(type)
, m_compressed(false)
{
	m_suffix = QString(".%1.%2.%3.%4")
		.arg(m_width).arg(m_height).arg(formatSuffix(m_format)).arg(typeSuffix(m_type));
}

FileNameSuffix::FileNameSuffix(
    const int width, const int height, const gl::GLenum compressedType)
: m_width(width)
, m_height(height)
, m_format(gl::GL_INVALID_ENUM)
, m_type(compressedType)
, m_compressed(true)
{
    m_suffix = QString(".%1.%2.%3")
        .arg(m_width).arg(m_height).arg(typeSuffix(m_type));
}

bool FileNameSuffix::isValid() const
{
	return m_width  != -1
		&& m_height != -1
        && m_type   != gl::GL_INVALID_ENUM
        && (m_compressed || m_format != gl::GL_INVALID_ENUM);
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

const QString & FileNameSuffix::get() const
{
	return m_suffix;
}

const QString FileNameSuffix::formatSuffix(const gl::GLenum format)
{
	return suffixesByFormat.value(format, "");
}

gl::GLenum FileNameSuffix::format(const QString & format)
{
    return suffixesByFormat.key(format.toLower(), gl::GL_INVALID_ENUM);
}

const QString FileNameSuffix::typeSuffix(const gl::GLenum type)
{
	return suffixesByType.value(type, "");
}

gl::GLenum FileNameSuffix::type(const QString & type)
{
    return suffixesByType.key(type.toLower(), gl::GL_INVALID_ENUM);
}

} // namespace glraw
