
#include <gloperate-qt/widgets/ScreenCapturerResolutionWidget.h>

#include <cmath>

#include <gloperate/ext-includes-begin.h>
#include "ui_ScreenCapturerResolutionWidget.h"
#include <gloperate/ext-includes-end.h>

#include <gloperate/tools/ScreenCapturer.h>


#define CM_PER_INCH 2.54
#define INCH_PER_CM 1 / CM_PER_INCH


namespace gloperate_qt
{

ScreenCapturerResolutionWidget::ScreenCapturerResolutionWidget(gloperate::ScreenCapturer * screenCapturer, QWidget * parent)
:   QWidget(parent)
,   m_screenCapturer(screenCapturer)
,   m_ui(new Ui_ScreenCapturerResolutionWidget)
{
    m_ui->setupUi(this);

    void (QComboBox:: *comboBoxCurrentIndexChanged)(const QString &) = &QComboBox::currentIndexChanged;
    connect(m_ui->widthComboBox, comboBoxCurrentIndexChanged,
        this, &ScreenCapturerResolutionWidget::widthUnitChanged);
    connect(m_ui->heightComboBox, comboBoxCurrentIndexChanged,
        this, &ScreenCapturerResolutionWidget::heightUnitChanged);
    connect(m_ui->resolutionComboBox, comboBoxCurrentIndexChanged,
        this, &ScreenCapturerResolutionWidget::resolutionUnitChanged);

    void (QDoubleSpinBox:: *doubleSpinBoxValueChanged)(double d) = &QDoubleSpinBox::valueChanged;
    void (QSpinBox:: *spinBoxValueChanged)(int i) = &QSpinBox::valueChanged;
    connect(m_ui->widthDoubleSpinBox, doubleSpinBoxValueChanged,
        this, &ScreenCapturerResolutionWidget::widthValueChanged);
    connect(m_ui->heightDoubleSpinBox, doubleSpinBoxValueChanged,
        this, &ScreenCapturerResolutionWidget::heightValueChanged);
    connect(m_ui->resolutionSpinBox, spinBoxValueChanged,
        this, &ScreenCapturerResolutionWidget::resolutionValueChanged);

    m_ui->widthDoubleSpinBox->setRange(0.01, 100000.0);
    m_ui->heightDoubleSpinBox->setRange(0.01, 100000.0);

    QStringList units{ QString::fromStdString(gloperate::ScreenCapturer::s_pixelString), QString::fromStdString(gloperate::ScreenCapturer::s_inchString), QString::fromStdString(gloperate::ScreenCapturer::s_cmString) };
    m_ui->widthComboBox->addItems(units);
    m_ui->heightComboBox->addItems(units);

    QStringList resolutions{ QString::fromStdString(gloperate::ScreenCapturer::s_ppiString), QString::fromStdString(gloperate::ScreenCapturer::s_pixelsPerCmString) };
    bool oldComboBoxSignalStatus = m_ui->resolutionComboBox->blockSignals(true);
    m_ui->resolutionComboBox->addItems(resolutions);
    m_ui->resolutionComboBox->blockSignals(oldComboBoxSignalStatus);

    bool oldSpinBoxSignalStatus = m_ui->resolutionSpinBox->blockSignals(true);
    m_ui->resolutionSpinBox->setValue(72);
    m_ui->resolutionSpinBox->blockSignals(oldSpinBoxSignalStatus);
}

void ScreenCapturerResolutionWidget::setDecimals(QDoubleSpinBox* box, int dec)
{
    bool old = box->blockSignals(true);
    box->setDecimals(dec);
    box->blockSignals(old);
}

void ScreenCapturerResolutionWidget::widthUnitChanged(const QString& text)
{
    if (text == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
    {
        if (m_ui->heightComboBox->currentText() == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
            enableResolution(false);
        
        setDecimals(m_ui->widthDoubleSpinBox, 0);
    }
    else if (!m_ui->resolutionLabel->isEnabled())
    {
        setDecimals(m_ui->widthDoubleSpinBox, 2);
        enableResolution(true);
    }

    bool old = m_ui->widthDoubleSpinBox->blockSignals(true);
    m_ui->widthDoubleSpinBox->setValue(m_screenCapturer->pixelsTo(m_screenCapturer->width(), m_ui->widthComboBox->currentText().toStdString()));
    m_ui->widthDoubleSpinBox->blockSignals(old);
}

void ScreenCapturerResolutionWidget::heightUnitChanged(const QString& text)
{
    if (text == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
    {
        if (m_ui->widthComboBox->currentText() == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
            enableResolution(false);
        
        setDecimals(m_ui->heightDoubleSpinBox, 0);
    }
    else if (!m_ui->resolutionLabel->isEnabled())
    {
        setDecimals(m_ui->heightDoubleSpinBox, 2);
        enableResolution(true);
    }

    bool old = m_ui->heightDoubleSpinBox->blockSignals(true);
    m_ui->heightDoubleSpinBox->setValue(m_screenCapturer->pixelsTo(m_screenCapturer->height(), m_ui->heightComboBox->currentText().toStdString()));
    m_ui->heightDoubleSpinBox->blockSignals(old);
}

void ScreenCapturerResolutionWidget::widthValueChanged(double d)
{
    if (m_ui->aspectCheckBox->isChecked())
    {
        m_screenCapturer->setHeight(std::round(m_screenCapturer->toPixels(d, m_ui->widthComboBox->currentText().toStdString()) * m_screenCapturer->height() / m_screenCapturer->width()));

        bool old = m_ui->heightDoubleSpinBox->blockSignals(true);
        m_ui->heightDoubleSpinBox->setValue(m_screenCapturer->pixelsTo(m_screenCapturer->height(), m_ui->heightComboBox->currentText().toStdString()));
        m_ui->heightDoubleSpinBox->blockSignals(old);
    }

    m_screenCapturer->setWidth(std::round(m_screenCapturer->toPixels(d, m_ui->widthComboBox->currentText().toStdString())));

    m_screenCapturer->createResolutionSummary();
}

void ScreenCapturerResolutionWidget::heightValueChanged(double d)
{
    if (m_ui->aspectCheckBox->isChecked())
    {
        m_screenCapturer->setWidth(std::round(m_screenCapturer->toPixels(d, m_ui->heightComboBox->currentText().toStdString()) * m_screenCapturer->width() / m_screenCapturer->height()));

        bool old = m_ui->widthDoubleSpinBox->blockSignals(true);
        m_ui->widthDoubleSpinBox->setValue(m_screenCapturer->pixelsTo(m_screenCapturer->height(), m_ui->widthComboBox->currentText().toStdString()));
        m_ui->widthDoubleSpinBox->blockSignals(old);
    }

    m_screenCapturer->setHeight(std::round(m_screenCapturer->toPixels(d, m_ui->heightComboBox->currentText().toStdString())));

    m_screenCapturer->createResolutionSummary();
}

void ScreenCapturerResolutionWidget::resolutionValueChanged(int i)
{
    double value;
    bool old;
    if (m_ui->aspectCheckBox->isChecked())
    {
        if (m_ui->widthComboBox->currentText() == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
        {
            value = m_screenCapturer->pixelsTo(m_screenCapturer->width(), m_ui->heightComboBox->currentText().toStdString());
            m_screenCapturer->setResolutionValue(i);
            m_screenCapturer->setWidth(std::round(m_screenCapturer->toPixels(value, m_ui->heightComboBox->currentText().toStdString())));

            old = m_ui->widthDoubleSpinBox->blockSignals(true);
            m_ui->widthDoubleSpinBox->setValue(m_screenCapturer->width());
            m_ui->widthDoubleSpinBox->blockSignals(old);
        }
        else if (m_ui->heightComboBox->currentText() == QString::fromStdString(gloperate::ScreenCapturer::s_pixelString))
        {
            value = m_screenCapturer->pixelsTo(m_screenCapturer->height(), m_ui->widthComboBox->currentText().toStdString());
            m_screenCapturer->setResolutionValue(i);
            m_screenCapturer->setHeight(std::round(m_screenCapturer->toPixels(value, m_ui->widthComboBox->currentText().toStdString())));

            old = m_ui->heightDoubleSpinBox->blockSignals(true);
            m_ui->heightDoubleSpinBox->setValue(m_screenCapturer->height());
            m_ui->heightDoubleSpinBox->blockSignals(old);
        }
    }
    else
        m_screenCapturer->setResolutionValue(i);

    m_screenCapturer->createResolutionSummary();
}

void ScreenCapturerResolutionWidget::resolutionUnitChanged(const QString& text)
{
    if (text == QString::fromStdString(gloperate::ScreenCapturer::s_ppiString))
        m_screenCapturer->setResolutionValue(std::ceil(m_screenCapturer->resolutionState().value * CM_PER_INCH));
    else
        m_screenCapturer->setResolutionValue(std::ceil(m_screenCapturer->resolutionState().value * INCH_PER_CM));

    bool old = m_ui->resolutionSpinBox->blockSignals(true);
    m_ui->resolutionSpinBox->setValue((int)m_screenCapturer->resolutionState().value);
    m_ui->resolutionSpinBox->blockSignals(old);
    m_screenCapturer->setResolutionUnit(text.toStdString());
}

void ScreenCapturerResolutionWidget::enableResolution(bool enable)
{
    m_ui->resolutionSpinBox->setEnabled(enable);
    m_ui->resolutionComboBox->setEnabled(enable);
    m_ui->resolutionLabel->setEnabled(enable);
}

ScreenCapturerResolutionWidget::~ScreenCapturerResolutionWidget()
{
}


} // namespace gloperate_qt
