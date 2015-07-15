#pragma once

#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>

#include <memory>

class Ui_PluginWidget;

namespace gloperate
{

class PluginManager;
class Plugin;

}

namespace gloperate_qt
{

class GLOPERATE_QT_API PluginWidget : public QWidget
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
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent * dropEvent);

protected:
	std::shared_ptr<gloperate::PluginManager> m_pluginManager;

private:
	const QScopedPointer<Ui_PluginWidget> m_ui;
	

};

} //namespace gloperate_qt
