#pragma once


#include <QString>

#include <glbinding/gl/gl.h>


namespace glraw
{


class FileNameSuffix
{
public:
	FileNameSuffix(const QString & fileName);

	FileNameSuffix(
		int width,
		int height,
        gl::GLenum format,
        gl::GLenum type);

	FileNameSuffix(
		int width,
		int height,
        gl::GLenum compressedType);

	/** \return Returns true, if width, height, type, and format if not compressed have valid values.
	*/
	bool isValid() const;

	/** \return Returns width of raw texture file name suffix in pixel;
				Is -1 when no width is provided.
	*/
	int width()  const;

	/** \return Returns height of raw texture file name suffix in pixel; 
				Is -1 when no height is provided.
	*/
	int height() const;

	/** \return Returns type of raw texture file name suffix; 
				Is GL_INVALID_ENUM when type is not unknown or not provided.
	*/
    gl::GLenum type() const;

	/** \return Returns format of raw texture file name suffix; 
				Is GL_INVALID_ENUM when format is not unknown or not provided.
	*/
    gl::GLenum format() const;

	/** \return Returns true if texture type is compressed and no extra format is given; false otherwise.
	    \remark When type is of GL_INVALID_ENUM, returns false.
	*/
	bool compressed() const;

	/** \return raw texture file name suffix as QString.
	*/
	const QString & get() const;

protected:
    static const QString formatSuffix(gl::GLenum format);
    static const QString typeSuffix(gl::GLenum type);

    static gl::GLenum format(const QString & suffix);
    static gl::GLenum type(const QString & suffix);

protected:
	QString m_suffix;

	int m_width;
	int m_height;
	
    gl::GLenum m_format;
    gl::GLenum m_type;

    bool m_compressed;
};

} // namespace glraw
