#include <gloperate-qt/Converter.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QGLWidget>
#include <gloperate-qt/qt-includes-end.h>
 

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
