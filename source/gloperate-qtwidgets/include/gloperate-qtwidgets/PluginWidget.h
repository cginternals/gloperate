#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

#include <memory>

class Ui_PluginWidget;

namespace gloperate
{

class PluginManager;
class Plugin;

}

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API PluginWidget : public QWidget
{
	Q_OBJECT

public:
	PluginWidget(std::shared_ptr<gloperate::PluginManager> pluginManager, QWidget * parent = nullptr);
	virtual ~PluginWidget();

signals:
    void pluginChanged(gloperate::Plugin&);

protected:
    virtual void initializeListView();
    virtual void cellSelected(int nRow, int);

protected:
	std::shared_ptr<gloperate::PluginManager> m_pluginManager;

private:
	const QScopedPointer<Ui_PluginWidget> m_ui;
	

};

} //namespace gloperate_qtwidgets
