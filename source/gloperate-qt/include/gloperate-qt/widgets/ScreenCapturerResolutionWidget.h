
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


/**
*  @brief
*    UI for resolution configuration of image saving.
*/
namespace gloperate_qt
{


class GLOPERATE_QT_API ScreenCapturerResolutionWidget : public QWidget
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] screenCapturer
    *    ScreenCapturer tool managed by UI, null_ptr is not allowed
    *  @param[in] parent
    *    Parent widget, null_ptr is allowed
    */
    ScreenCapturerResolutionWidget(gloperate::ScreenCapturer * screenCapturer, QWidget * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScreenCapturerResolutionWidget();


private:
    void widthUnitChanged(const QString& text);
    void heightUnitChanged(const QString& text);
    void resolutionUnitChanged(const QString& text);
    void enableResolution(bool enable);
    void widthValueChanged(double d);
    void heightValueChanged(double d);
    void resolutionValueChanged(int i);

    void setDecimals(QDoubleSpinBox* box, int dec);


private:
    gloperate::ScreenCapturer * m_screenCapturer;
    const QScopedPointer<Ui_ScreenCapturerResolutionWidget> m_ui;
};


} // namespace gloperate_qt
