
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ScreenCapturerTilebasedWidget;


namespace gloperate_qt
{


class GLOPERATE_QT_API ScreenCapturerTilebasedWidget : public QWidget
{
    Q_OBJECT


public:
    ScreenCapturerTilebasedWidget(QWidget * parent = nullptr);
    virtual ~ScreenCapturerTilebasedWidget();


private:
    const QScopedPointer<Ui_ScreenCapturerTilebasedWidget> m_ui;
};


} // namespace gloperate_qt
