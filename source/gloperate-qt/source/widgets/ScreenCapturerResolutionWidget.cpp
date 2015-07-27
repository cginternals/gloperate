
#include <gloperate-qt/widgets/ScreenCapturerResolutionWidget.h>

#include <cmath>

#include <gloperate/ext-includes-begin.h>
#include "ui_ScreenCapturerResolutionWidget.h"
#include <gloperate/ext-includes-end.h>


#define CM_PER_INCH 2.54
#define INCH_PER_CM 1 / CM_PER_INCH


namespace gloperate_qt
{


const QString ppiString = "ppi";
const QString pixelsPerCmString = "px/cm";
const QString pixelString = "pixel";
const QString inchString = "inch";
const QString cmString = "cm";


ScreenCapturerResolutionWidget::ScreenCapturerResolutionWidget(QWidget * parent)
:   QWidget(parent)
,   m_width(1920)
,   m_height(1080)
,   m_resolutionState(new ResolutionState(72, ppiString))
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

    QStringList units{ pixelString, inchString, cmString };
    m_ui->widthComboBox->addItems(units);
    m_ui->heightComboBox->addItems(units);

    QStringList resolutions{ ppiString, pixelsPerCmString };
    bool oldComboBoxSignalStatus = m_ui->resolutionComboBox->blockSignals(true);
    m_ui->resolutionComboBox->addItems(resolutions);
    m_ui->resolutionComboBox->blockSignals(oldComboBoxSignalStatus);

    bool oldSpinBoxSignalStatus = m_ui->resolutionSpinBox->blockSignals(true);
    m_ui->resolutionSpinBox->setValue(72);
    m_ui->resolutionSpinBox->blockSignals(oldSpinBoxSignalStatus);
}

void ScreenCapturerResolutionWidget::updateResolutionSummary()
{
    // TODO: detect unsigned long long overflow
    emit resolutionChanged(QSize(m_width, m_height));

    unsigned long long pixelNumber{ static_cast<unsigned long long>(m_width) * static_cast<unsigned long long>(m_height) };
    QString unit;
    double byte;
    if (pixelNumber * 4 < 1024)
    {
        unit = "Byte";
        byte = static_cast<double>(pixelNumber) * 4;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 2)))
    {
        unit = "KiB";
        byte = static_cast<double>(pixelNumber) * 4 / 1024;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 3)))
    {
        unit = "MiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 2);
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 4)))
    {
        unit = "GiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 3);
    }
    else //if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 5)))
    {
        unit = "TiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 4);
    }
    QString summary{ QString::number(pixelNumber) + " pixels (" + QString::number(std::round(byte * 100) / 100) + " " + unit + " uncompressed)" };
    //m_ui->resolutionSummaryLabel->setText(summary);
    emit resolutionSummaryChanged(summary);
}

double ScreenCapturerResolutionWidget::inchToPixels(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= m_resolutionState->value * CM_PER_INCH;

    return value;
}

double ScreenCapturerResolutionWidget::cmToPixels(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= m_resolutionState->value * INCH_PER_CM;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= m_resolutionState->value;

    return value;
}

double ScreenCapturerResolutionWidget::toPixels(double value, const QString& type)
{
    if (type == inchString)
        value = inchToPixels(value);
    else if (type == cmString)
        value = cmToPixels(value);

    return value;
}

double ScreenCapturerResolutionWidget::pixelsToCm(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= CM_PER_INCH / m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value /= m_resolutionState->value;

    return value;
}

double ScreenCapturerResolutionWidget::pixelsToInch(double value)
{
    if (m_resolutionState->type == ppiString)
        value /= m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= INCH_PER_CM / m_resolutionState->value;

    return value;
}

double ScreenCapturerResolutionWidget::pixelsTo(double value, const QString& type)
{
    if (type == inchString)
        value = pixelsToInch(value);
    else if (type == cmString)
        value = pixelsToCm(value);

    return value;
}

void ScreenCapturerResolutionWidget::setDecimals(QDoubleSpinBox* box, int dec)
{
    bool old = box->blockSignals(true);
    box->setDecimals(dec);
    box->blockSignals(old);
}

void ScreenCapturerResolutionWidget::widthUnitChanged(const QString& text)
{
    if (text == pixelString)
    {
        if (m_ui->heightComboBox->currentText() == pixelString)
            enableResolution(false);
        
        setDecimals(m_ui->widthDoubleSpinBox, 0);
    }
    else if (!m_ui->resolutionLabel->isEnabled())
    {
        setDecimals(m_ui->widthDoubleSpinBox, 2);
        enableResolution(true);
    }

    bool old = m_ui->widthDoubleSpinBox->blockSignals(true);
    m_ui->widthDoubleSpinBox->setValue(pixelsTo(m_height, m_ui->widthComboBox->currentText()));
    m_ui->widthDoubleSpinBox->blockSignals(old);
}

void ScreenCapturerResolutionWidget::heightUnitChanged(const QString& text)
{
    if (text == pixelString)
    {
        if (m_ui->widthComboBox->currentText() == pixelString)
            enableResolution(false);
        
        setDecimals(m_ui->heightDoubleSpinBox, 0);
    }
    else if (!m_ui->resolutionLabel->isEnabled())
    {
        setDecimals(m_ui->heightDoubleSpinBox, 2);
        enableResolution(true);
    }

    bool old = m_ui->heightDoubleSpinBox->blockSignals(true);
    m_ui->heightDoubleSpinBox->setValue(pixelsTo(m_height, m_ui->heightComboBox->currentText()));
    m_ui->heightDoubleSpinBox->blockSignals(old);
}

void ScreenCapturerResolutionWidget::widthValueChanged(double d)
{
    if (m_ui->aspectCheckBox->isChecked())
    {
        m_height = std::round(toPixels(d, m_ui->widthComboBox->currentText()) * m_height / m_width);

        bool old = m_ui->heightDoubleSpinBox->blockSignals(true);
        m_ui->heightDoubleSpinBox->setValue(pixelsTo(m_height, m_ui->heightComboBox->currentText()));
        m_ui->heightDoubleSpinBox->blockSignals(old);
    }

    m_width = std::round(toPixels(d, m_ui->widthComboBox->currentText()));

    updateResolutionSummary();
}

void ScreenCapturerResolutionWidget::heightValueChanged(double d)
{
    if (m_ui->aspectCheckBox->isChecked())
    {
        m_width = std::round(toPixels(d, m_ui->heightComboBox->currentText()) * m_width / m_height);

        bool old = m_ui->widthDoubleSpinBox->blockSignals(true);
        m_ui->widthDoubleSpinBox->setValue(pixelsTo(m_height, m_ui->widthComboBox->currentText()));
        m_ui->widthDoubleSpinBox->blockSignals(old);
    }

    m_height = std::round(toPixels(d, m_ui->heightComboBox->currentText()));

    updateResolutionSummary();
}

void ScreenCapturerResolutionWidget::resolutionValueChanged(int i)
{
    double value;
    bool old;
    if (m_ui->aspectCheckBox->isChecked())
    {
        if (m_ui->widthComboBox->currentText() == pixelString)
        {
            value = pixelsTo(m_width, m_ui->heightComboBox->currentText());
            m_resolutionState->value = i;
            m_width = std::round(toPixels(value, m_ui->heightComboBox->currentText()));

            old = m_ui->widthDoubleSpinBox->blockSignals(true);
            m_ui->widthDoubleSpinBox->setValue(m_width);
            m_ui->widthDoubleSpinBox->blockSignals(old);
        }
        else if (m_ui->heightComboBox->currentText() == pixelString)
        {
            value = pixelsTo(m_height, m_ui->widthComboBox->currentText());
            m_resolutionState->value = i;
            m_height = std::round(toPixels(value, m_ui->widthComboBox->currentText()));

            old = m_ui->heightDoubleSpinBox->blockSignals(true);
            m_ui->heightDoubleSpinBox->setValue(m_height);
            m_ui->heightDoubleSpinBox->blockSignals(old);
        }
    }
    else
        m_resolutionState->value = i;

    updateResolutionSummary();
}

void ScreenCapturerResolutionWidget::resolutionUnitChanged(const QString& text)
{
    if (text == ppiString)
        m_resolutionState->value = std::ceil(m_resolutionState->value * CM_PER_INCH);
    else
        m_resolutionState->value = std::ceil(m_resolutionState->value * INCH_PER_CM);

    bool old = m_ui->resolutionSpinBox->blockSignals(true);
    m_ui->resolutionSpinBox->setValue((int)m_resolutionState->value);
    m_ui->resolutionSpinBox->blockSignals(old);
    m_resolutionState->type = text;
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
