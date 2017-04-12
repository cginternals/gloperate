#pragma once

#include <string>

#include <glbinding/gl/types.h>


namespace glraw
{


/**
*  @brief
*    Class to parse the filename of .raw files and extract the attributes of the texture.
*
*/
class FileNameSuffix
{
public:  
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] fileName
    *    filename of the .raw file
    */
    FileNameSuffix(const std::string & fileName);


    /**
    * @brief
    *   Return true, if width, height, type, and format if not compressed have valid values.
    */
    bool isValid() const;

    /**
    * @brief
    *   Return width of raw texture file name suffix in pixel;
    *   Is -1 when no width is provided.
    */
    int width()  const;

    /**
    * @brief
    *   Return height of raw texture file name suffix in pixel;
    *   Is -1 when no height is provided.
    */
    int height() const;

    /**
    * @brief
    *   Returns type of raw texture file name suffix;
    *   Is GL_NONE when type is not unknown or not provided.
    */
    gl::GLenum type() const;

    /**
    * @brief
    *   Returns format of raw texture file name suffix;
    *   Is GL_NONE when format is not unknown or not provided.
    */
    gl::GLenum format() const;

    /**
    * @brief
    *   Returns true if texture type is compressed and no extra format is given; false otherwise.
    *   When type is of GL_NONE, returns false.
    */
    bool compressed() const;

protected:
    /**
    *  @brief
    *    find format enum that corresponds to suffix
    *
    *  @param[in] suffix
    *    part of a .raw filename
    */
    static gl::GLenum format(const std::string & suffix);

    /**
    *  @brief
    *    find type enum that corresponds to suffix
    *
    *  @param[in] suffix
    *    part of a .raw filename
    */
    static gl::GLenum type(const std::string & suffix);

protected:
    int m_width;  ///< Width of the Texture
    int m_height; ///< Height of the Texture

    gl::GLenum m_format; ///< Format of the Texture
    gl::GLenum m_type;   ///< Type of the Texture

    bool m_compressed;   ///< states if Texture is compressed or not
};

} // namespace glraw
