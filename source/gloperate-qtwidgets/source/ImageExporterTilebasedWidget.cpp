#include <gloperate-qtwidgets/ImageExporterTilebasedWidget.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_ImageExporterTilebasedWidget.h"
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qtwidgets
{

ImageExporterTilebasedWidget::ImageExporterTilebasedWidget(QWidget *parent)
:	QWidget(parent)
,	m_ui(new Ui_ImageExporterTilebasedWidget)
{
    m_ui->setupUi(this);
}

ImageExporterTilebasedWidget::~ImageExporterTilebasedWidget()
{
}

} //namespace gloperate_qtwidgets
