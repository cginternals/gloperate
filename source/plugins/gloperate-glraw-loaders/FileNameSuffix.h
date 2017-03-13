#pragma once


#include <glbinding/gl/gl.h>


namespace glraw
{


class FileNameSuffix
{
public:
    FileNameSuffix(const std::string & fileName);


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

    /** \return raw texture file name suffix.
	*/
    const std::string & get() const;

protected:
    static gl::GLenum format(const std::string & suffix);
    static gl::GLenum type(const std::string & suffix);

protected:
    std::string m_suffix;

	int m_width;
	int m_height;
	
    gl::GLenum m_format;
    gl::GLenum m_type;

    bool m_compressed;
};

} // namespace glraw
