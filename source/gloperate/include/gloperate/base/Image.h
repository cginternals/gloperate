
#pragma once


#include <glbinding/gl/types.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/gloperate_features.h>


namespace gloperate
{


/**
*  @brief
*    Image class that holds image data and information
*
*    Currently supports format RGB24.
*/
class GLOPERATE_API Image
{
public:
    /**
    *  @brief
    *    Get number of color channels for given image format
    *
    *  @param[in] format
    *    Format (OpenGL definition)
    *
    *  @return
    *    Number of color channels
    */
    static int channels(gl::GLenum format);

    /**
    *  @brief
    *    Get number of bytes for a given data type
    *
    *  @param[in] type
    *    Data type (OpenGL definition)
    *
    *  @return
    *    Number of bytes per element
    */
    static int bytes(gl::GLenum type);


public:
    /**
    *  @brief
    *    Constructor
    *
    *    Constructs an empty image
    *
    *  @see empty()
    */
    Image();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *
    *  @remarks
    *    Allocates the necessary image data memory.
    *
    *  @see createBuffer
    */
    Image(int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *  @param[in] data
    *    Pointer to image data (must NOT be nullptr!)
    *
    *  @remarks
    *    This allocates new image memory and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    *
    *  @see createBuffer
    */
    Image(int width, int height, gl::GLenum format, gl::GLenum type, const char * data);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *  @param[in] data
    *    Pointer to image data (must NOT be nullptr!)
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    */
    Image(int width, int height, gl::GLenum format, gl::GLenum type, char * data);

    /**
    *  @brief
    *    Copy constructor
    *
    *  @param[in] other
    *    Source image
    */
    Image(const Image & other);

    /**
    *  @brief
    *    Move constructor
    *
    *  @param[in] other
    *    Source image
    */
    Image(Image && other);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Image();

    /**
    *  @brief
    *    Assignment operator
    *
    *  @param[in] other
    *    Source image
    *
    *  @return
    *    Reference to this object

    *  @see
    *    https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    */
    Image & operator=(Image other);

    /**
    *  @brief
    *    Check if image is empty
    *
    *  @return
    *    'true' if the image is empty, else 'false'
    *
    *  @remarks
    *    An empty image has all parameters set to zero and no allocated data.
    */
    bool empty() const;

    /**
    *  @brief
    *    Get width
    *
    *  @return
    *    Image width
    */
    int width() const;

    /**
    *  @brief
    *    Get height
    *
    *  @return
    *    Image height
    */
    int height() const;

    /**
    *  @brief
    *    Get image format
    *
    *  @return
    *    Image format (OpenGL definition)
    */
    gl::GLenum format() const;

    /**
    *  @brief
    *    Get data type
    *
    *  @return
    *    Data type (OpenGL definition)
    */
    gl::GLenum type() const;

    /**
    *  @brief
    *    Get number of color channels
    *
    *  @return
    *    Number of color channels
    */
    int channels() const;

    /**
    *  @brief
    *    Get number of bytes per element
    *
    *  @return
    *    Number of bytes per element
    */
    int bytes() const;

    /**
    *  @brief
    *    Get image data
    *
    *  @return
    *    Pointer to raw image data (can be null)
    */
    const char * data() const;

    /**
    *  @brief
    *    Get image data
    *
    *  @return
    *    Pointer to raw image data (can be null)
    */
    char * data();

    /**
    *  @brief
    *    Clear image
    *
    *  @remarks
    *    Releases all data and resets to an empty image.
    *
    *  @see empty()
    */
    void clear();

    /**
    *  @brief
    *    Allocate image memory
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *
    *  @remarks
    *    Any existing image data is deleted.
    */
    void allocate(int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Create image from existing image data
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *  @param[in] data
    *    Pointer to image data (must NOT be nullptr!)
    *
    *  @remarks
    *    This allocates new image memory and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    *    Any existing image data is deleted.
    */
    void copyImage(int width, int height, gl::GLenum format, gl::GLenum type, const char * data);

    /**
    *  @brief
    *    Set image data
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *  @param[in] data
    *    Pointer to image data (must NOT be nullptr!)
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    *    Any existing image data is deleted.
    */
    void setData(int width, int height, gl::GLenum format, gl::GLenum type, char * data);

    /**
    *  @brief
    *    Swap function for copy-and-swap idiom
    *
    *  @param[in] first
    *    Image
    *  @param[in] second
    *    Image
    *
    *  @remarks
    *    See https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    */
    friend void swap(Image & first, Image & second) GLOPERATE_NOEXCEPT;


protected:
    /**
    *  @brief
    *    Set image information
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format (OpenGL definition)
    *  @param[in] type
    *    Data type (OpenGL definition)
    *
    *  @remarks
    *    This function only sets the image information, it does not
    *    allocate or set the image data.
    */
    void initializeImage(int width, int height, gl::GLenum format, gl::GLenum type);


protected:
    int        m_width;     ///< Image width (0 if empty)
    int        m_height;    ///< Image height (0 if empty)
    gl::GLenum m_format;    ///< Image format (OpenGL definition, GL_INVALID_ENUM if empty)
    gl::GLenum m_type;      ///< Data type (OpenGL definition, GL_INVALID_ENUM if empty)
    int        m_channels;  ///< Number of color channels (0 if empty)
    int        m_bytes;     ///< Bytes per element (0 if empty)
    int        m_dataSize;  ///< Size of image data (0 if empty)
    char *     m_data;      ///< Image data (can be null)
};


} // namespace gloperate
