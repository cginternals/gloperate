
#include <gloperate-qt/widgets/ImageExporterMultiFrameWidget.h>

#include <gloperate/ext-includes-begin.h>
#include "ui_ImageExporterMultiFrameWidget.h"
#include <gloperate/ext-includes-end.h>


namespace gloperate_qt
{


ImageExporterMultiFrameWidget::ImageExporterMultiFrameWidget(QWidget *parent)
: QWidget(parent)
, m_ui(new Ui_ImageExporterMultiFrameWidget)
{
    m_ui->setupUi(this);

    connect(m_ui->multiFrameCountSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(frameCountChanged(int)));
}

ImageExporterMultiFrameWidget::~ImageExporterMultiFrameWidget()
{
}


} // namespace gloperate_qt
