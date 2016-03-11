
#pragma once


#include <glbinding/gl/enum.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    gloperate image class, that holds image data and information.
*
*    Currently supports format RGB24.
*/
class GLOPERATE_API Image
{
public:
    /**
    *  @brief
    *    Constructor. Constructs a null image (\see{isNull})
    */
    Image();

    /**
    *  @brief
    *    Copy-Constructor
    */
    Image(const Image & other);

    /**
    *  @brief
    *    Move-Constructor
    */
    Image(Image && other);

    /**
    *  @brief
    *    Constructor. Allocates data memory (\see{createBuffer})
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    */
    Image(int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Constructor. Allocates data memory (\see{createBuffer}) fills it with content of \p data
    *
    *  @param[in] data
    *    Const pointer to image data
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    *
    *  @remarks
    *    This allocates own memory, and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    */
    Image(const char * data, int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Constructor. Takes over the ownership of \p data
    *
    *  @param[in] data
    *    Pointer to image data
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    */
    Image(char * data, int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Destructor. Frees the memory associated with \a m_data
    */
    virtual ~Image();

    /**
    *  @brief
    *    Assignment operator
    *
    *  @remarks
    *    \see https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    */
    Image& operator=(Image other);

    /**
    *  @brief
    *    Allocates memory for image data
    *
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    *
    *  @remarks
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand
    */
    void createBuffer(int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Allocates data memory (\see{createBuffer}) fills it with content of \p data
    *
    *  @param[in] data
    *    Const pointer to image data
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    *
    *  @remarks
    *    This allocates own memory, and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand.
    */
    void setData(const char * data, int width, int height, gl::GLenum format, gl::GLenum type);

    /**
    *  @brief
    *    Takes over the ownership of \p data
    *
    *  @param[in] data
    *    Pointer to image data
    *  @param[in] width
    *    Image width
    *  @param[in] height
    *    Image height
    *  @param[in] format
    *    Image format
    *  @param[in] type
    *    Image type
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand.
    */
    void setData(char * data, int width, int height, gl::GLenum format, gl::GLenum type);

    // copy constructor, copy operator

    /**
    *  @brief
    *    Returns true if it is a null image, otherwise returns false
    *
    *  @remarks
    *    A null image has all parameters set to zero and no allocated data
    */
    bool isNull() const;

    /**
    *  @brief
    *    Format getter
    *
    *  @return
    *    Format of stored image data
    */
    gl::GLenum format() const;

    /**
    *  @brief
    *    Type getter
    *
    *  @return
    *    Type of stored image data
    */
    gl::GLenum type() const;

    /**
    *  @brief
    *    Getter for image data pointer (can be nullptr)
    *
    *  @return
    *    Data pointer of stored image data (can be nullptr)
    */
    char * data();

    /**
    *  @brief
    *    Const getter for image data pointer (can be nullptr)
    *
    *  @return
    *    Const data pointer of stored image data (can be nullptr)
    */
    const char * data() const;

    /**
    *  @brief
    *    Width getter
    *
    *  @return
    *    Width of stored image
    */
    int width() const;

    /**
    *  @brief
    *    Height getter
    *
    *  @return
    *    Height of stored image
    */
    int height() const;

    /**
    *  @brief
    *    Get Number of channels
    *
    *  @return
    *    Number of channels of stored image
    */
    int channels() const;

    /**
    *  @brief
    *    Type size getter
    *
    *  @return
    *    Size of the data type of stored image
    */
    int typeSize() const;

    /**
    *  @brief
    *    Swap function for copy-and-swap idiom
    *
    *  @remarks
    *    See https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    */
    friend void swap(Image & first, Image & second) noexcept;


protected:
    void setImageData(int width, int height, gl::GLenum format, gl::GLenum type);
    static int channels(gl::GLenum format);
    static int typeSize(gl::GLenum type);


protected:
    char *     m_data;
    int        m_dataSize;
    int        m_width;
    int        m_height;
    int        m_channels;
    int        m_typeSize;
    gl::GLenum m_format;
    gl::GLenum m_type;
};


} // namespace gloperate
