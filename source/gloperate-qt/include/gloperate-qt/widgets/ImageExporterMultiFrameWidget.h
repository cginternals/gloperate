
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ImageExporterMultiFrameWidget;


namespace gloperate_qt
{


class GLOPERATE_QT_API ImageExporterMultiFrameWidget : public QWidget
{
    Q_OBJECT


public:
    ImageExporterMultiFrameWidget(QWidget * parent = nullptr);
    virtual ~ImageExporterMultiFrameWidget();


signals:
    void frameCountChanged(int count);


private:
    const QScopedPointer<Ui_ImageExporterMultiFrameWidget> m_ui;
};


} // namespace gloperate_qt
