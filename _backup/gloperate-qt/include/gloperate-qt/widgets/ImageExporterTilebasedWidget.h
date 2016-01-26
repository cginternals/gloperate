
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ImageExporterTilebasedWidget;


namespace gloperate_qt
{


class GLOPERATE_QT_API ImageExporterTilebasedWidget : public QWidget
{
    Q_OBJECT


public:
    ImageExporterTilebasedWidget(QWidget * parent = nullptr);
    virtual ~ImageExporterTilebasedWidget();


private:
    const QScopedPointer<Ui_ImageExporterTilebasedWidget> m_ui;
};


} // namespace gloperate_qt
