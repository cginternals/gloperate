
#include <gloperate-qt/widgets/PluginPathWidget.h>

#include <gloperate/base/make_unique.hpp>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>

#include <gloperate/ext-includes-begin.h>
#include <loggingzeug/logging.h>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QGridLayout>
#include <gloperate/ext-includes-end.h>


using gloperate::make_unique;

namespace gloperate_qt
{


PluginPathWidget::PluginPathWidget(gloperate::PluginManager * pluginManager, QWidget *parent)
: QWidget(parent)
, m_pluginManager(pluginManager)
, m_listWidget(new QListWidget(this))
, m_browsePathButton(new QPushButton(this))
, m_removePathButton(new QPushButton(this))
{
    m_listWidget->setWindowTitle("Plugin search paths");
    m_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (auto & path : m_pluginManager->paths())
    {
        m_listWidget->addItem(QString::fromStdString(path));
    }

    m_browsePathButton->setText("Add");
    m_removePathButton->setText("Remove");

    auto layout = new QGridLayout();
    this->setLayout(layout);

    layout->addWidget(m_listWidget.get(), 0, 0, 1, 2);
    layout->addWidget(m_browsePathButton.get(), 1, 0);
    layout->addWidget(m_removePathButton.get(), 1, 1);


    QObject::connect(m_browsePathButton.get(), &QPushButton::released, [this, pluginManager] () 
        {
            const auto path = QFileDialog::getExistingDirectory(this, windowTitle(), QString{}, {});

            if (path.isEmpty())
                return;
            
            pluginManager->addPath(path.toStdString());
            m_listWidget->addItem(path);
        });

    QObject::connect(m_removePathButton.get(), &QPushButton::released, [this, pluginManager] () 
        {
            for (auto & item : m_listWidget->selectedItems())
            {
                pluginManager->removePath(item->text().toStdString());
                item->setHidden(true);
            }
        });
}

PluginPathWidget::~PluginPathWidget()
{
}


} // namespace gloperate_qt
