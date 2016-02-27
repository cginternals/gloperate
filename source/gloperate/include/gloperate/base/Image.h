
#pragma once


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
    enum Format
    {
        Invalid,
        RGB24
    };

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
    */
    Image(const int width, const int height, const Image::Format format);

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
    *
    *  @remarks
    *    This allocates own memory, and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    */
    Image(const char * data, const int width, const int height, const Image::Format format);

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
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    */
    Image(char * data, const int width, const int height, const Image::Format format);

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
    *
    *  @remarks
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand
    */
    void createBuffer(const int width, const int height, const Image::Format format);

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
    *
    *  @remarks
    *    This allocates own memory, and copies the content of \p data.
    *    The ownership of \p data remains at the caller.
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand.
    */
    void setData(const char * data, const int width, const int height, const Image::Format format);

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
    *
    *  @remarks
    *    This does NOT allocate own memory.
    *    The ownership of \p data is transferred to the Image object.
    *    If \a m_data is an existing memory buffer, this buffer is deleted beforehand.
    */
    void setData(char * data, const int width, const int height, const Image::Format format);

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
    Format format() const;

    /**
    *  @brief
    *    Getter for image data pointer (can be nullptr)
    *
    *  @return
    *    Data pointer of stored image data (can be nullptr)
    */
    char * data() const;

    /**
    *  @brief
    *    Const getter for image data pointer (can be nullptr)
    *
    *  @return
    *    Const data pointer of stored image data (can be nullptr)
    */
    const char * cdata() const;

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
    *    Swap function for copy-and-swap idiom
    *
    *  @remarks
    *    See https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    */
    friend void swap(Image & first, Image & second) noexcept;


protected:
    void setImageData(const int width, const int height, const Image::Format format);
    static int computeDataSize(const int width, const int height, const Image::Format format);


protected:
    char * m_data;
    int    m_dataSize;
    int    m_width;
    int    m_height;
    Format m_format;
};


} // namespace gloperate
