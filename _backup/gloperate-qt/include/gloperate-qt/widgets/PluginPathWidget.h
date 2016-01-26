
#pragma once


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <gloperate/ext-includes-end.h>


#include <gloperate-qt/gloperate-qt_api.h>


class QListWidget;
class QPushButton;

namespace gloperate
{
    class PluginManager;
}

namespace gloperate_qt
{


class GLOPERATE_QT_API PluginPathWidget : public QWidget
{
    Q_OBJECT


public:
    PluginPathWidget(gloperate::PluginManager * pluginManager, QWidget * parent = nullptr);
    virtual ~PluginPathWidget();

protected:
    void addPath(const QString & path);
    void dragEnterEvent(QDragEnterEvent * event) override;
    void dropEvent(QDropEvent * event) override;



private:
    gloperate::PluginManager * m_pluginManager;
    std::unique_ptr<QListWidget> m_listWidget;
    std::unique_ptr<QPushButton> m_browsePathButton;
    std::unique_ptr<QPushButton> m_removePathButton;
};


} // namespace gloperate_qt
