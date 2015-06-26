
#include <gloperate-qt/widgets/ImageExporterTilebasedWidget.h>

#include <gloperate/ext-includes-begin.h>
#include "ui_ImageExporterTilebasedWidget.h"
#include <gloperate/ext-includes-end.h>


namespace gloperate_qt
{


ImageExporterTilebasedWidget::ImageExporterTilebasedWidget(QWidget *parent)
: QWidget(parent)
, m_ui(new Ui_ImageExporterTilebasedWidget)
{
    m_ui->setupUi(this);
}

ImageExporterTilebasedWidget::~ImageExporterTilebasedWidget()
{
}


} // namespace gloperate_qt
