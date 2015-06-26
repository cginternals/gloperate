
#include <gloperate-qt/viewer/Converter.h>

#include <gloperate/ext-includes-begin.h>
#include <QGLWidget>
#include <gloperate/ext-includes-end.h>
 

namespace gloperate_qt
{


/**
*  @brief
*    Convert image into OpenGL compatible format
*/
QImage Converter::convert(const QImage &image)
{
    return QGLWidget::convertToGLFormat(image);
}


} // namespace gloperate_qt
