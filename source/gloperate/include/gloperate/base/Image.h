
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    
*/
class GLOPERATE_API Image
{
public:
    enum Format
    {
        RGB24,
        Invalid
    };

public:
    /**
    *  @brief
    *    Constructor
    */
    Image();

    Image(const Image::Format format, char * data, const int width, const int height);

    Image(const Image::Format format, const int width, const int height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Image();

    void setImage(const Image::Format format, char * data, const int width, const int height);

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
    *    Image data getter
    *
    *  @return
    *    Data pointer of stored image
    */
    char * data();

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


protected:
    Format m_format;
    char * m_data;
    int    m_width;
    int    m_height;
};


} // namespace gloperate
