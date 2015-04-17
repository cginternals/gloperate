#include <gloperate-qtwidgets/PluginWidget.h>
#include "ui_PluginWidget.h"

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QAbstractButton>
#include <QWindow>
#include <gloperate-qt/qt-includes-end.h>

#include <cassert>

namespace gloperate_qtwidgets
{

PluginWidget::PluginWidget(std::shared_ptr<gloperate::PluginManager> pluginManager, QWidget *parent)
:	QWidget(parent)
,	m_ui(new Ui_PluginWidget)
,	m_pluginManager(pluginManager)
{
	m_ui->setupUi(this);

    initializeListView();
}

PluginWidget::~PluginWidget()
{
}

void PluginWidget::initializeListView()
{
    assert(m_pluginManager);

    m_ui->pluginTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_ui->pluginTableWidget->setRowCount(m_pluginManager->plugins().size());

    QStringList tableHeader;
    tableHeader << "Name" << "Version" << "Description" << "Type" << "Vendor";
    m_ui->pluginTableWidget->setHorizontalHeaderLabels(tableHeader);

    int pluginCount = 0;

    for (auto plugin : m_pluginManager->plugins())
    {
        m_ui->pluginTableWidget->setItem(pluginCount, 0, new QTableWidgetItem(plugin->name()));
        m_ui->pluginTableWidget->setItem(pluginCount, 1, new QTableWidgetItem(plugin->version()));
        m_ui->pluginTableWidget->setItem(pluginCount, 2, new QTableWidgetItem(plugin->description()));
        m_ui->pluginTableWidget->setItem(pluginCount, 3, new QTableWidgetItem(plugin->type()));
        m_ui->pluginTableWidget->setItem(pluginCount++, 4, new QTableWidgetItem(plugin->vendor()));
    }

    connect(m_ui->pluginTableWidget, &QTableWidget::cellDoubleClicked,
        this, &PluginWidget::cellSelected);
}

void PluginWidget::cellSelected(int nRow, int)
{
    emit pluginChanged(*m_pluginManager->plugins().at(nRow));
}

} //namespace gloperate_qtwidgets
