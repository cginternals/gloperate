
#pragma once


#include <string>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Class to parse the filename of .raw files and extract the attributes of the texture.
*/
class GLOPERATE_API RawFileNameSuffix
{
public:  
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] fileName
    *    filename of the .raw file
    */
    RawFileNameSuffix(const std::string & fileName);

    /**
    *  @brief
    *    Check if filename is valid
    *
    *  @return
    *    Return true, if width, height, type, and format if not compressed have valid values.
    */
    bool isValid() const;

    /**
    *  @brief
    *    Get texture with
    *
    *  @return
    *    Width of raw texture file name suffix in pixels, -1 when no width is provided.
    */
    int width()  const;

    /**
    *  @brief
    *    Get texture height
    *
    *  @return
    *    Height of raw texture file name suffix in pixels, -1 when no height is provided.
    */
    int height() const;

    /**
    *  @brief
    *    Check if texture is compressed
    *
    *  @return
    *    'true' if texture type is compressed and no extra format is given, 'false' otherwise.
    *    When type is of GL_NONE, returns false.
    */
    bool compressed() const;

    /**
    *  @brief
    *    Get texture type
    *
    *  @return
    *    Type of raw texture file name suffix, GL_NONE when type is not unknown or not provided.
    */
    gl::GLenum type() const;

    /**
    *  @brief
    *    Get texture format
    *
    *  @return
    *    Format of raw texture file name suffix, GL_NONE when format is not unknown or not provided.
    */
    gl::GLenum format() const;


protected:
    /**
    *  @brief
    *    Find format enum that corresponds to suffix
    *
    *  @param[in] suffix
    *    part of a .raw filename
    *
    *  @return
    *    OpenGL format
    */
    static gl::GLenum format(const std::string & suffix);

    /**
    *  @brief
    *    Find type enum that corresponds to suffix
    *
    *  @param[in] suffix
    *    part of a .raw filename
    *
    *  @return
    *    OpenGL format
    */
    static gl::GLenum type(const std::string & suffix);


protected:
    int        m_width;      ///< Width of the Texture
    int        m_height;     ///< Height of the Texture
    bool       m_compressed; ///< states if Texture is compressed or not
    gl::GLenum m_format;     ///< Format of the Texture
    gl::GLenum m_type;       ///< Type of the Texture
};


} // namespace gloperate
