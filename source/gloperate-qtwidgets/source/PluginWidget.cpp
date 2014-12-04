#include <gloperate-qtwidgets/PluginWidget.h>
#include "ui_PluginWidget.h"

#include <gloperate/plugin/PluginManager.h>

#include <QAbstractButton>
#include <QWindow>

namespace gloperate_qtwidgets
{

PluginWidget::PluginWidget(gloperate::PluginManager * pluginManager, QWidget *parent)
:	QWidget(parent)
,	m_ui(new Ui_PluginWidget)
,	m_pluginManager(pluginManager)
{
	m_ui->setupUi(this);
}

PluginWidget::~PluginWidget()
{
}

} //namespace gloperate_qtwidgets
