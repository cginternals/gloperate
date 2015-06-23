#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_ImageExporterTilebasedWidget;

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API ImageExporterTilebasedWidget : public QWidget
{
    Q_OBJECT

public:
    ImageExporterTilebasedWidget(QWidget * parent = nullptr);
    virtual ~ImageExporterTilebasedWidget();

private:
    const QScopedPointer<Ui_ImageExporterTilebasedWidget> m_ui;
    

};

} //namespace gloperate_qtwidgets
