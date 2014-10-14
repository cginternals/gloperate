#pragma once


#include <gloperate-qt/qt-includes-begin.h>
#include <QImage>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API Converter
{


    public:
        static QImage convert(const QImage &image);


};


} // namespace gloperate_qt
