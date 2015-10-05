
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QImage>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Tool class to convert a QImage into OpenGL compatible format
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
    static QImage convert(const QImage &image);
};


} // namespace gloperate_qt
