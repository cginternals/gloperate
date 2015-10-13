
#include <gloperate-qt/widgets/PluginWidget.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>
#include <gloperate/plugin/PainterPlugin.h>
#include <gloperate/plugin/PluginLibrary.h>

#include <gloperate/ext-includes-begin.h>

#include <QAbstractButton>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHeaderView>
#include <QMimeData>
#include <QResizeEvent>
#include <QString>
#include <QStringList>
#include <QWindow>
#include <QDebug>

#include "ui_PluginWidget.h"

#include <gloperate/ext-includes-end.h>

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

namespace gloperate_qt
{

PluginWidget::PluginWidget(gloperate::PluginManager * pluginManager, gloperate::ResourceManager * resourceManager, QWidget *parent)
:   QWidget(parent)
,   m_pluginManager(pluginManager)
,   m_resourceManager(resourceManager)
,   m_ui(new Ui_PluginWidget)
{
    assert(m_pluginManager);

    m_ui->setupUi(this);

    initializeListView();

    setAcceptDrops(true);
    m_ui->pluginTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

    m_pluginManager->pluginsChanged.connect([=](){updateListView(); });
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

	updateListView();

    connect(m_ui->pluginTableWidget, &QTableWidget::cellDoubleClicked,
        this, &PluginWidget::painterSelected);
}

void PluginWidget::updateListView()
{
    assert (m_pluginManager);

	m_ui->pluginTableWidget->setRowCount(m_pluginManager->plugins().size());

	QStringList tableHeader;
	tableHeader << "Name" << "Version" << "Description" << "Type" << "Vendor" << "Filepath";
	m_ui->pluginTableWidget->setHorizontalHeaderLabels(tableHeader);

	int pluginCount = 0;

    std::vector<gloperate::PluginLibrary *> pluginLibraries{ m_pluginManager->pluginLibraries() };

    for (auto pluginLibrary : pluginLibraries)
	{
        for (size_t pluginIndex = 0; pluginIndex < pluginLibrary->numPlugins(); pluginIndex++)
        {
            m_ui->pluginTableWidget->setItem(pluginCount, 0, new QTableWidgetItem(pluginLibrary->plugin(pluginIndex)->name()));
            m_ui->pluginTableWidget->setItem(pluginCount, 1, new QTableWidgetItem(pluginLibrary->plugin(pluginIndex)->version()));
            m_ui->pluginTableWidget->setItem(pluginCount, 2, new QTableWidgetItem(pluginLibrary->plugin(pluginIndex)->description()));
            m_ui->pluginTableWidget->setItem(pluginCount, 3, new QTableWidgetItem(pluginLibrary->plugin(pluginIndex)->type()));
            m_ui->pluginTableWidget->setItem(pluginCount, 4, new QTableWidgetItem(pluginLibrary->plugin(pluginIndex)->vendor()));
            m_ui->pluginTableWidget->setItem(pluginCount++, 5, new QTableWidgetItem(QString::fromStdString(pluginLibrary->filePath())));
        }
	}
}

void PluginWidget::painterSelected(int nRow, int)
{
    assert(m_pluginManager);
    assert(m_resourceManager);

    gloperate::Plugin * plugin{ m_pluginManager->plugins().at(nRow) };
    emit painterChanged(*dynamic_cast<gloperate::AbstractPainterPlugin *>(plugin)->createPainter(*m_resourceManager));
}

void PluginWidget::dropEvent(QDropEvent * dropEvent)
{
    assert(m_pluginManager);

	int len = dropEvent->mimeData()->data("text/uri-list").length();
	QString uri = dropEvent->mimeData()->data("text/uri-list").right(len - 8);
	QString filename = uri.left(uri.length() - 2);
	qDebug() << filename;
    m_pluginManager->load(filename.toStdString(), false);
	m_ui->pluginTableWidget->clear();
	updateListView();
}

void PluginWidget::resizeEvent(QResizeEvent * resizeEvent)
{
    int tableSize = m_ui->pluginTableWidget->width();

    int numberOfColumns = m_ui->pluginTableWidget->columnCount();
    int innerTableSize{ 0 };
    for (int i = 0; i < numberOfColumns; i++)
        innerTableSize += m_ui->pluginTableWidget->columnWidth(i);

    int difference = resizeEvent->size().width() - resizeEvent->oldSize().width() + (tableSize - innerTableSize);
    int delta = std::floor(difference / numberOfColumns);

    for (int columnIndex = 0; columnIndex < numberOfColumns - 1; columnIndex++)
    {
        m_ui->pluginTableWidget->setColumnWidth(columnIndex, m_ui->pluginTableWidget->columnWidth(columnIndex) + delta);
    }

    QWidget::resizeEvent(resizeEvent);
}

} //namespace gloperate_qt
