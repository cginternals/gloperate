#include <gloperate-qtwidgets/PluginWidget.h>
#include "ui_PluginWidget.h"

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QAbstractButton>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QString>
#include <QStringList>
#include <QWindow>
#include <QDebug>
#include <gloperate-qt/qt-includes-end.h>

#include <cassert>
#include <ostream>

namespace
{
// Define system specific filename properties
#ifdef WIN32
const int RTLD_LAZY(0); // ignore for win32 - see dlopen
const std::string g_ext = "dll";
#elif __APPLE__
const std::string g_ext = "dylib";
#else
const std::string g_ext = "so";
#endif
}

namespace gloperate_qtwidgets
{

PluginWidget::PluginWidget(std::shared_ptr<gloperate::PluginManager> pluginManager, QWidget *parent)
:	QWidget(parent)
,	m_pluginManager(pluginManager)
,	m_ui(new Ui_PluginWidget)
{
	m_ui->setupUi(this);

    initializeListView();

	setAcceptDrops(true);
}

PluginWidget::~PluginWidget()
{
}

void PluginWidget::dragEnterEvent(QDragEnterEvent *event)
{
	QString uri = event->mimeData()->data("text/uri-list");
	int len = uri.length();

	if (uri.mid(len - g_ext.length() - 3).left(g_ext.length() + 1).toLower() == "." + QString::fromStdString(g_ext))
		event->acceptProposedAction();
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

void PluginWidget::dropEvent(QDropEvent * dropEvent)
{
	int len = dropEvent->mimeData()->data("text/uri-list").length();
	QString uri = dropEvent->mimeData()->data("text/uri-list").right(len - 8);
	QString filename = uri.left(uri.length() - 2);
	qDebug() << filename;
	m_pluginManager->loadLibrary(filename.toStdString(), false);
	m_ui->pluginTableWidget->clear();
	initializeListView();
}
} //namespace gloperate_qtwidgets
