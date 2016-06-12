
#pragma once


#include <QImage>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Utility class to convert a QImage into OpenGL compatible format
*/
class GLOPERATE_QT_API Converter
{
public:
    /**
    *  @brief
    *    Convert image into OpenGL compatible format
    *
    *  @param[in] image
    *    Input image
    *
    *  @return
    *    Image in GL format
    */
    static QImage convert(const QImage & image);
};


} // namespace gloperate_qt
