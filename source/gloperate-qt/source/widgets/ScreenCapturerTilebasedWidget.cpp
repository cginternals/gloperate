
#include <gloperate-qt/widgets/ScreenCapturerTilebasedWidget.h>

#include <gloperate/ext-includes-begin.h>
#include "ui_ScreenCapturerTilebasedWidget.h"
#include <gloperate/ext-includes-end.h>


namespace gloperate_qt
{


ScreenCapturerTilebasedWidget::ScreenCapturerTilebasedWidget(QWidget *parent)
:   QWidget(parent)
,   m_ui(new Ui_ScreenCapturerTilebasedWidget)
{
    m_ui->setupUi(this);
}

ScreenCapturerTilebasedWidget::~ScreenCapturerTilebasedWidget()
{
}


} // namespace gloperate_qt
