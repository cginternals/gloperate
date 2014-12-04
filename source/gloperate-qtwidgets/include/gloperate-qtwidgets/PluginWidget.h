#pragma once

#include <QWidget>
#include <QScopedPointer>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_PluginWidget;

namespace gloperate
{

class PluginManager;

}

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API PluginWidget : public QWidget
{
	Q_OBJECT

public:
	PluginWidget(gloperate::PluginManager * pluginManager, QWidget * parent = nullptr);
	virtual ~PluginWidget();

protected:
	gloperate::PluginManager * m_pluginManager;

private:
	const QScopedPointer<Ui_PluginWidget> m_ui;
	

};

} //namespace gloperate_qtwidgets
