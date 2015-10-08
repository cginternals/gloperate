
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>

#include <memory>


class Ui_PluginWidget;
class QResizeEvent;

namespace gloperate
{

class PluginManager;
class Painter;
class ResourceManager;

}


namespace gloperate_qt
{


class GLOPERATE_QT_API PluginWidget : public QWidget
{
	Q_OBJECT

public:
	PluginWidget(gloperate::PluginManager * pluginManager, gloperate::ResourceManager * resourceManager, QWidget * parent = nullptr);
	virtual ~PluginWidget();

signals:
    void painterChanged(gloperate::Painter&);

protected:
    virtual void initializeListView();
	virtual void updateListView();
    virtual void painterSelected(int nRow, int);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent * dropEvent);

    virtual void resizeEvent(QResizeEvent * resizeEvent);

protected:
    gloperate::PluginManager * m_pluginManager;
    gloperate::ResourceManager * m_resourceManager;

private:
	const QScopedPointer<Ui_PluginWidget> m_ui;
	

};

} //namespace gloperate_qt
