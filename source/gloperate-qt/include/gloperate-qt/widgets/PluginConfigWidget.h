
#pragma once


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QTabWidget>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class PluginManager;
    class ResourceManager;
}


namespace gloperate_qt
{


class PluginWidget;
class PluginPathWidget;


class GLOPERATE_QT_API PluginConfigWidget : public QTabWidget
{
    Q_OBJECT


public:
    PluginConfigWidget(gloperate::PluginManager * pluginManager, gloperate::ResourceManager * resourceManager, QWidget * parent = nullptr);
    virtual ~PluginConfigWidget();


private:
    std::unique_ptr<PluginWidget> m_pluginWidget;
    std::unique_ptr<PluginPathWidget> m_pluginPathWidget;
};


} // namespace gloperate_qt
