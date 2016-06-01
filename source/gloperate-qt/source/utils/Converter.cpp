
#include <gloperate-qt/utils/Converter.h>

#include <QGLWidget>
 

namespace gloperate_qt
{


QImage Converter::convert(const QImage & image)
{
    return QGLWidget::convertToGLFormat(image);
}


} // namespace gloperate_qt
