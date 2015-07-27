
#pragma once


#include <memory>

#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ScreenCapturerResolutionWidget;
class QDoubleSpinBox;


namespace gloperate_qt
{


struct ResolutionState {
    ResolutionState(double d, QString s)
    : value{d}
    , type{s}
    {
    }

    double value;
    QString type;
};


class GLOPERATE_QT_API ScreenCapturerResolutionWidget : public QWidget
{
    Q_OBJECT


public:
    ScreenCapturerResolutionWidget(QWidget * parent = nullptr);
    virtual ~ScreenCapturerResolutionWidget();

    void updateResolutionSummary();


signals:
    void resolutionSummaryChanged(const QString& resolutionSummary);
    void resolutionChanged(const QSize& resolution);


protected:
    void widthUnitChanged(const QString& text);
    void heightUnitChanged(const QString& text);
    void resolutionUnitChanged(const QString& text);
    void enableResolution(bool enable);
    void widthValueChanged(double d);
    void heightValueChanged(double d);
    void resolutionValueChanged(int i);

    double inchToPixels(double value);
    double cmToPixels(double value);
    double toPixels(double value, const QString& type);
    double pixelsToCm(double value);
    double pixelsToInch(double value);
    double pixelsTo(double value, const QString& type);

    void setDecimals(QDoubleSpinBox* box, int dec);


protected:
    int m_width;
    int m_height;
    std::unique_ptr<ResolutionState> m_resolutionState;


private:
    const QScopedPointer<Ui_ScreenCapturerResolutionWidget> m_ui;
};


} // namespace gloperate_qt
