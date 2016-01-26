
#include <gloperate-qt/widgets/PluginConfigWidget.h>

#include <gloperate/base/make_unique.hpp>

#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/Plugin.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/PluginWidget.h>
#include <gloperate-qt/widgets/PluginPathWidget.h>

#include <gloperate/ext-includes-begin.h>
#include <loggingzeug/logging.h>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <gloperate/ext-includes-end.h>


using gloperate::make_unique;

namespace gloperate_qt
{


PluginConfigWidget::PluginConfigWidget(gloperate::PluginManager * pluginManager, gloperate::ResourceManager * resourceManager, QWidget *parent)
: QTabWidget(parent)
, m_pluginWidget(new PluginWidget(pluginManager, resourceManager, this))
, m_pluginPathWidget(new PluginPathWidget(pluginManager, this))
{
    addTab(m_pluginWidget.get(), "Plugins");
    addTab(m_pluginPathWidget.get(), "Paths");
}

PluginConfigWidget::~PluginConfigWidget()
{
}


} // namespace gloperate_qt
