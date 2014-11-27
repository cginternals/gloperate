#include <gloperate-qtwidgets/Dummy.h>

#include <cassert>

#include "ui_Dummy.h"


namespace gloperate_qtwidgets
{

Dummy::Dummy(QWidget * parent)
:   QWidget(parent)
,   m_ui(new Ui_Dummy)
{
    m_ui->setupUi(this);
}

Dummy::~Dummy()
{
}

} // namespace gloperate_qtwidgets
