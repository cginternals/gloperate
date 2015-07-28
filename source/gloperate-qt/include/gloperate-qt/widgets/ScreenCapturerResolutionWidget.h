
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


namespace gloperate
{

class ScreenCapturer;

}


namespace gloperate_qt
{


class GLOPERATE_QT_API ScreenCapturerResolutionWidget : public QWidget
{
    Q_OBJECT


public:
    ScreenCapturerResolutionWidget(gloperate::ScreenCapturer * screenCapturer, QWidget * parent = nullptr);
    virtual ~ScreenCapturerResolutionWidget();


protected:
    void widthUnitChanged(const QString& text);
    void heightUnitChanged(const QString& text);
    void resolutionUnitChanged(const QString& text);
    void enableResolution(bool enable);
    void widthValueChanged(double d);
    void heightValueChanged(double d);
    void resolutionValueChanged(int i);

    void setDecimals(QDoubleSpinBox* box, int dec);


protected:
    gloperate::ScreenCapturer * m_screenCapturer;


private:
    const QScopedPointer<Ui_ScreenCapturerResolutionWidget> m_ui;
};


} // namespace gloperate_qt
